from socket import socket
from threading import Timer

from mod_protocol import Protocol
from mod_iconnection import IConnection
from mod_logger import MOD_LOG, LogLevel


class StreamMasterConnection(IConnection):

    def __init__(self, configPath = None):
        self.__address = None
        self.__port = None
        self.__socket = None
        self.__name = "StreamMasterConnection"
        self.__keepAliveTimer = Timer(15.0, self.__checkConnection)

        if configPath is None:
            self.initializeDefault()
        else:
            self.initializeFromConfig(configPath)

        self.connect()
        self.__keepAliveTimer.start()

    def __checkConnection(self):
        MOD_LOG("checkConnection", LogLevel.INFO)
        if self.send(Protocol.Request.CheckConnection) != Protocol.Replays.OK:
            return self.connect()
        return Protocol.Replays.OK    

    @property
    def name(self):
        return self.__name

    @property
    def state(self):
        return self.send(Protocol.Request.State, 4)

    def connect(self):
        try:
            self.__socket = socket()
            self.__socket.connect((self.__address, self.__port))
            MOD_LOG("Connection successful")
            return Protocol.Replays.OK
        except:
            MOD_LOG("Connection failure", LogLevel.ERROR)
            return Protocol.Replays.NotAvailable

    def disconnect(self):
        self.__socket.close()

    def initializeDefault(self):
        self.__address = '127.0.0.1'
        self.__port = 48684

    def initializeFromConfig(self, configPath):
        pass

    def send(self, message, response_length=2):
        return self.__safeRemoteExec(message, response_length)

    def __safeRemoteExec(self, command, response_length=2):
        try:
            MOD_LOG("try to send: " + command)
            replay = self.__send(command, response_length)
            MOD_LOG("Replay: " + replay)
            return replay
        except:
            return Protocol.Replays.NotAvailable

    def __send(self, command, response_length):
        self.__socket.send(command)
        return self.__socket.recv(response_length)

    def updateService(self, service):
        MOD_LOG("try to update service")
        return self.send(Protocol.Request.UpdateService + str(len(service)) + service)

    def updateToken(self, token):
        MOD_LOG("try to update token")
        return self.send(Protocol.Request.UpdateToken + str(len(token)) + token)

    def startStream(self):
        MOD_LOG("try to start stream")
        return self.send(Protocol.Request.StartStream)

    def stopStream(self):
        MOD_LOG("try to stop stream")
        return self.send(Protocol.Request.StopStream) 
