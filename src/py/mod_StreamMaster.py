import re
import atexit
import string
import subprocess
from os import path
from socket import socket
from abc import ABCMeta, abstractmethod

from gui.Scaleform.framework import g_entitiesFactories, ViewSettings
from gui.Scaleform.framework import ViewTypes, ScopeTemplates
from gui.Scaleform.framework.managers.loaders import SFViewLoadParams
from gui.app_loader import g_appLoader
from gui.shared.utils.key_mapping import getBigworldNameFromKey
from gui.Scaleform.framework.entities.View import View
from gui.shared.utils.graphics import g_monitorSettings

from gui import InputHandler


def gl_Log(message):
    print 'MOD_LOG: ' + message


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


class ConnectionState:
    NotConnected = 0
    Connected = 1


class IConnection(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def connect(self):
        raise NotImplementedError

    @abstractmethod
    def disconnect(self):
        raise NotImplementedError


class StreamMasterConnection(IConnection):

    def __init__(self, configPath = None):
        self.__protocol = Protocol()
        self.__address = None
        self.__port = None
        self.__socket = None
        self.__name = "StreamMasterConnection"
        self.__state = ConnectionState.NotConnected

        if configPath is None:
            self.initializeDefault()
        else:
            self.initializeFromConfig(configPath)

        self.tryToConnect()

    def tryToConnect(self):
        if self.connect() is self.__protocol.unavailable:
            if self.__runExternalServer() is self.__protocol.unavailable:
                gl_Log("Connection failed!")
                return self.__protocol.unavailable

        self.__state = ConnectionState.Connected
        return self.__protocol.ok


    @property
    def name(self):
        return self.__name

    @property
    def state(self):
        return self.__state

    def connect(self):
        try:
            self.__socket = socket()
            self.__socket.connect((self.__address, self.__port))
            return self.__protocol.ok
        except:
            return self.__protocol.unavailable


    def disconnect(self):
        self.__socket.close()
        self.__state = ConnectionState.Connected

    def initializeDefault(self):
        self.__address = '127.0.0.1'
        self.__port = 48684

    def initializeFromConfig(self, configPath):
        pass

    def send(self, message, response_length=2):
        self.__safeRemoteExec(message, response_length)

    def __runExternalServer(self):
        wsr_path = path.abspath(path.join(__file__, r'..\..\..\..\..\\res_mods\1.0.2.1\StreamMaster\\'))
        full_name = wsr_path + r'\StreamMaster.exe'
        sinfo = subprocess.STARTUPINFO()
        sinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        try:
            wsr = subprocess.Popen(full_name, cwd=wsr_path, startupinfo=sinfo)
            atexit.register(wsr.kill)
            gl_Log("external server started")
            return self.__protocol.ok
        except:
            gl_Log("external server can't start")
            return self.__protocol.unavailable


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
        gl_Log("Connections number: " + str(len(self.__connections)))

    @property
    def count(self):
        return len(self.__connections)

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
        if self.__connections[0].state is ConnectionState.NotConnected:
            self.__connections[0].tryToConnect()

        return self.__connections[0].send('stst')

    def stopStream(self):
        return self.__connections[0].send('spst')


class StreamService(object):
    def __init__(self):
        self._name = "None"
        self._pattern = None
        self._token = ''

    @property
    def pattern(self):
        return self._pattern

    def setPattern(self, pattern):
        self._pattern = pattern

    @property
    def token(self):
        return self._token

    def setToken(self, token):
        self._token = token

    @property
    def name(self):
        return self._name


class YouTubeService(StreamService):
    def __init__(self):
        super(YouTubeService, self).__init__()
        self._name = "YouTube"
        self._pattern = re.compile(r'^[a-z0-9]{4,4}-[a-z0-9]{4,4}-[a-z0-9]{4,4}-[a-z0-9]{4,4}', re.IGNORECASE)


class TwitchService(StreamService):
    def __init__(self):
        super(TwitchService, self).__init__()
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

    def setService(self, service):
        self.__service = service

    @property
    def connectionManager(self):
        return self.__connectionManager

    @property
    def state(self):
        return self.__state

    def setState(self, state):
        self.__state = state


g_streamMaster = StreamMasterSingleton()


class OverlayWindow(View):

    def __init__(self):
        super(OverlayWindow, self).__init__()
        global g_streamMaster
        self.__streamMaster = g_streamMaster
        self.__smConnection = self.__streamMaster.connectionManager.getConnectionByName("StreamMasterConnection")

    def _populate(self):
        self.log("OW: populate")
        super(OverlayWindow, self)._populate()

    def _dispose(self):
        self.log("OW: dispose")
        super(OverlayWindow, self)._dispose()

    def __del__(self):
        self.log("OW: del")
        super(OverlayWindow, self).__del__()

    def onWindowClose(self):
        self.log("OW: close")
        self.destroy()

    def onToggleStream(self):
        self.log("stream toggled")

        if self.__smConnection is None:
            self.flashObject.setStatusText('No connection')
            pass

        self.__streamMaster.connectionManager.updateToken(self.__streamMaster.service.token)

        if self.__streamMaster.state is not StreamState.Running:
            self.log("Sent: ")
            self.__streamMaster.connectionManager.startStream()
            self.__streamMaster.setState(StreamState.Running)
        else:
            self.log("Sent: ")
            self.__streamMaster.connectionManager.stopStream()

    def onServiceChanged(self):
        if self.flashObject.service() is "Twitch":
            self.__streamMaster.setService(TwitchService())
        else:
            self.__streamMaster.setService(YouTubeService())

        name = self.__streamMaster.service.name
        self.log('Current service is ' + name)

    def getStreamState(self):
        return self.__streamMaster.state

    @staticmethod
    def getClientWindowWidth():
        return g_monitorSettings.currentWindowSize.width

    @staticmethod
    def getClientWindowHeight():
        return g_monitorSettings.currentWindowSize.height

    @staticmethod
    def log(message):
        gl_Log(message)

    # -- from view
    def checkIfTokenValid(self):

        isValid = False
        token = self.flashObject.token()
        self.log("Current token is: " + token)

        if token and len(token) == 19:
            matched = self.__streamMaster.service.pattern.match(token)
            if matched:
                self.__streamMaster.service.setToken(token)
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
        OverlayWindow.log("f10 pressed")
        g_appLoader.getApp().loadView(SFViewLoadParams(_alias))


InputHandler.g_instance.onKeyDown += on_handle_key_event
