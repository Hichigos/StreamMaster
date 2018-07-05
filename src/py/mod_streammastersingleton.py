import atexit

from os import path
import subprocess

from mod_connectionmanager import ConnectionManager
from mod_streamservice import StreamService
from mod_protocol import Protocol

import mod_logger

class StreamState:
    Initialized = 1
    Running = 2
    Paused = 3


class StreamMasterSingleton(object):

    def __init__(self):
        self.__runExternalServer()

        self.__connectionManager = ConnectionManager()
        self.__service = StreamService()
        self.__state = StreamState.Initialized

    
    def __runExternalServer(self):
        wsr_path = path.abspath(path.join(__file__, r'..\..\..\..\..\\res_mods\1.0.2.2\StreamMaster\\'))
        full_name = wsr_path + r'\StreamMaster.exe'
        sinfo = subprocess.STARTUPINFO()
        # sinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        try:
            wsr = subprocess.Popen(full_name, cwd=wsr_path, startupinfo=sinfo)
            atexit.register(wsr.kill)
            mod_logger.MOD_LOG("external server started")
            return Protocol.Replays.OK
        except:
            mod_logger.MOD_LOG("external server failed to start", mod_logger.LogLevel.ERROR)
            return Protocol.Replays.NotAvailable

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