import re
import atexit
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


class ConnectionState(object):
    def __init__(self):
        self._isRunning = False
        self._token = ''


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
        self.__connectionState = ConnectionState()
        self.__protocol = Protocol()
        self.__address = None
        self.__port = None
        self.__socket = None

        if configPath == '':
            self.initializeDefault()
        else:
            self.initializeFromConfig(configPath)

        self.__runExternalServer()
        self.connect()

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
            return self.__protocol.unawailable

    def __send(self, command, response_length):
        self.__socket.send(command)
        return self.__socket.recv(response_length)


class ConnectionManager(object):

    def __init__(self):
        self.__connections = []
        self.createConnection()

    def createConnection(self):
        self.__connections.append(StreamMasterConnection())

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


g_connectionManager = ConnectionManager()


class OverlayWindow(View):

    def __init__(self):
        View.__init__(self)
        global g_connectionManager
        self.__state = g_connectionManager
        self.WSR = self.__state.WSR
        self.proto = self.WSR.proto

    def _populate(self):
        View._populate(self)

    def _dispose(self):
        View._dispose(self)

    def onWindowClose(self):
        self.destroy()

    def toggleStream(self):
        SystemMessages.pushMessage("toggled")

    @staticmethod
    def getClientWindowWidth():
        return g_monitorSettings.currentWindowSize.width

    @staticmethod
    def getClientWindowHeight():
        return g_monitorSettings.currentWindowSize.height

    @staticmethod
    def log(message):
        debug_utils.LOG_ERROR(message)

    # -- from view
    def checkInput(self, input):

        input_valid = False

        if input and len(input) == 19:
            match = self.__state.pattern.match(input)
            if match:
                self.__state.token = match.group(0)
                input_valid = True

        self.__onInputValidate(input_valid)

    # -- from view
    def startStopStream(self, token):

        state = self.WSR.getState()

        if state == self.proto.unawailable:
            self.__onConnect(self.WSR.connect())

        elif state == self.proto.notInitialized:
            self.__onInitialize(self.WSR.initialize())

        elif state == self.proto.stopped:
            self.WSR.updateToken(self.__state.token)
            self.__onStreamStart(self.WSR.startStream())

        elif state == self.proto.busy:
            self.__onBusy()

        elif state == self.proto.started:
            self.__onStreamStop(self.WSR.stopStream())
        else:
            pass


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
