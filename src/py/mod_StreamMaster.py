import re
import atexit
import string
import subprocess
from os import path
from socket import socket
import debug_utils
from abc import ABCMeta, abstractmethod

from gui.Scaleform.framework import g_entitiesFactories, ViewSettings
from gui.Scaleform.framework import ViewTypes, ScopeTemplates
from gui.Scaleform.framework.managers.loaders import SFViewLoadParams
from gui.app_loader import g_appLoader
from gui.shared.utils.key_mapping import getBigworldNameFromKey
from gui.Scaleform.framework.entities.View import View
from gui.shared.utils.graphics import g_monitorSettings

from gui import InputHandler
from gui import SystemMessages


class Protocol(object): # I Want Enums

    def __init__(self):

        # ordinary responses
        self.__ok = 'ok'
        self.__bp = 'bp'
        self.__er = 'er'
        self.__uw = 'uw'

        # 'stat' responses
        self.__ntin = 'ntin'
        self.__stpd = 'stpd'
        self.__busy = 'busy'
        self.__srtd = 'srtd'

    # ordinary responses

    @property
    def ok(self):
        return self.__ok

    @property
    def badOperation(self):
        return self.__bp

    @property
    def error(self):
        return self.__er

    @property
    def unavailable(self):
        return self.__uw

    # 'stat' responses

    @property
    def notInitialized(self):
        return self.__ntin

    @property
    def started(self):
        return self.__srtd

    @property
    def busy(self):
        return self.__busy

    @property
    def stopped(self):
        return self.__stpd


class IConnection(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def connect(self):
        raise NotImplementedError

    @abstractmethod
    def disconnect(self):
        raise NotImplementedError


class StreamMasterConnection(IConnection):

    def __init__(self, configPath = ''):
        self.__protocol = Protocol()
        self.__address = None
        self.__port = None
        self.__socket = None
        self.__name = None

        if configPath == '':
            self.initializeDefault()
        else:
            self.initializeFromConfig(configPath)

        # self.__runExternalServer()
        # self.connect()

    @property
    def name(self):
        return self.__name

    def connect(self):
        try:
            self.__socket = socket()
            self.__socket.connect((self.__address, self.__port))
            return self.__protocol.ok
        except:
            return self.__protocol.unavailable

    def disconnect(self):
        pass

    def initializeDefault(self):
        self.__address = '127.0.0.1'
        self.__port = 48684

    def initializeFromConfig(self, configPath):
        pass

    def send(self, message, response_length):
        self.__safeRemoteExec(message, response_length)

    def __runExternalServer(self):
        wsr_path = path.abspath(path.join(__file__, r'..\..\..\..\..\\res_mods\1.0.2.1\StreamMaster\\'))
        full_name = wsr_path + r'\StreamMaster.exe'
        sinfo = subprocess.STARTUPINFO()
        sinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        wsr = subprocess.Popen(full_name, cwd=wsr_path, startupinfo=sinfo)
        atexit.register(wsr.kill)

    def __safeRemoteExec(self, command, response_length=2):
        try:
            return self.__send(command, response_length)
        except:
            return self.__protocol.unavailable

    def __send(self, command, response_length):
        self.__socket.send(command)
        return self.__socket.recv(response_length)


class ConnectionManager(object):

    def __init__(self):
        self.__connections = []
        self.createConnection()

    def createConnection(self):
        self.__connections.append(StreamMasterConnection())

    def getConnectionByName(self, name):
        for connection in self.__connections:
            if connection.name is name:
                return connection
                
        return None
        
    def getState(self):
        return self.__connections[0].send('stat', 4)

    def initialize(self):
        return self.__connections[0].send('init')

    def updateToken(self, token):
        return self.__connections[0].send('updt: ' + token)

    def startStream(self):
        return self.__connections[0].send('srts')

    def stopStream(self):
        return self.__connections[0].send('stps')

class StreamService(object):
    def __init__(self):
        self._name = "None"
        self._pattern = ''
        self._token = ''

    @property
    def pattern(self):
        return self._pattern

    @property
    def token(self):
        return self._token


class YouTubeService(StreamService):
    def __init__(self):
        StreamService.__init__(self)
        self._name = "YouTube"
        self._pattern = re.compile(r'^[a-z0-9]{4,4}-[a-z0-9]{4,4}-[a-z0-9]{4,4}-[a-z0-9]{4,4}', re.IGNORECASE)


class TwitchService(StreamService):
    def __init__(self):
        StreamService.__init__(self)
        self._name = "Twitch"
        self._pattern = re.compile(r'^[a-z0-9]{4,4}-[a-z0-9]{4,4}-[a-z0-9]{4,4}-[a-z0-9]{4,4}', re.IGNORECASE)


class StreamState:
    Initialized = 1
    Running = 2
    Paused = 3

class StreamMasterSingleton(object):

    def __init__(self):
        self.__connectionManager = ConnectionManager()
        self.__service = StreamService()
        self.__state = StreamState.Initialized

    @property
    def service(self):
        return self.__service

    @property
    def connectionManager(self):
        return self.__connectionManager

    @property
    def state(self):
        return self.__state



g_streamMaster = StreamMasterSingleton()

class OverlayWindow(View):

    def __init__(self):
        super(OverlayWindow, self).__init__()
        View.__init__(self)
        global g_streamMaster
        self.__streamMaster = g_streamMaster
        self.__smConnection = self.__streamMaster.connectionManager.getConnectionByName("StreamMasterConnection")

    def _populate(self):
        super(OverlayWindow, self)._populate()

    def _dispose(self):
        super(OverlayWindow, self)._dispose()

    def __del__(self):
        super(OverlayWindow, self).__del__()

    def onWindowClose(self):
        self.destroy()

    def onToggleStream(self):
        self.log("stream toggled")

        if self.__smConnection is None:
            self.flashObject.setErrorMessage('No connection')
            pass

        if self.__streamMaster.state is not StreamState.Running:
            self.__smConnection.send("stst")
        else:
            self.__smConnection.send("spst")

    def onServiceChanged(self):
        if self.flashObject.service is "Twitch":
            self.__streamMaster.service = TwitchService()
        else:
            self.__streamMaster.service = YouTubeService()

    @staticmethod
    def getClientWindowWidth():
        return g_monitorSettings.currentBorderlessSize.width

    @staticmethod
    def getClientWindowHeight():
        return g_monitorSettings.currentBorderlessSize.height

    @staticmethod
    def log(message):
        debug_utils.LOG_DEBUG(message)

    # -- from view
    def checkIfTokenValid(self):

        isValid = False
        token = self.flashObject.token()

        if input and len(input) == 19:
            match = self.__streamMaster.service.pattern.match(token)
            if match:
                self.__streamMaster.service.token = match.group(0)
                isValid = True

        return isValid

    # -- from view
    # def startStopStream(self, token):

    #     state = self.WSR.getState()

    #     if state == self.proto.unawailable:
    #         self.__onConnect(self.WSR.connect())

    #     elif state == self.proto.notInitialized:
    #         self.__onInitialize(self.WSR.initialize())

    #     elif state == self.proto.stopped:
    #         self.WSR.updateToken(self.__state.token)
    #         self.__onStreamStart(self.WSR.startStream())

    #     elif state == self.proto.busy:
    #         self.__onBusy()

    #     elif state == self.proto.started:
    #         self.__onStreamStop(self.WSR.stopStream())
    #     else:
    #         pass


_alias = 'OverlayWindow'
_url = 'StreamMaster.swf'
_type = ViewTypes.WINDOW
_event = None
_scope = ScopeTemplates.GLOBAL_SCOPE

_settings = ViewSettings(_alias, OverlayWindow, _url, _type, _event, _scope)
g_entitiesFactories.addSettings(_settings)


def on_handle_key_event(event):
    key = getBigworldNameFromKey(event.key)

    if key == 'KEY_F10':
        g_appLoader.getApp().loadView(SFViewLoadParams(_alias))


InputHandler.g_instance.onKeyDown += on_handle_key_event
