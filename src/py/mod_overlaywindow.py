from gui.Scaleform.framework.entities.View import View
from gui.shared.utils.graphics import g_monitorSettings

from mod_protocol import Protocol
from mod_streamservice import YouTubeService, TwitchService, FacebookService
from mod_streammastersingleton import g_streamMaster
from mod_logger import MOD_LOG, LogLevel


class OverlayWindow(View):

    def __init__(self):
        super(OverlayWindow, self).__init__()
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

        if self.__smConnection.updateToken(self.__streamMaster.service.token) == Protocol.Replays.OK:
            state = self.__smConnection.state

            if state == Protocol.Replays.Stopped:
                self.__smConnection.startStream()  

            elif state == Protocol.Replays.Started:
                self.__smConnection.stopStream()

            else:
                self.flashObject.setStatusText("Can't toggle stream, try again later!")
        else:
            self.flashObject.setStatusText('failed to update token')
            MOD_LOG("Failed to update token", LogLevel.ERROR)
        self.flashObject.updateControls()

    def onServiceChanged(self, service):

        if self.__smConnection is None:
            self.flashObject.setStatusText('No connection')
            pass 

        if self.__smConnection.updateService(service) == Protocol.Replays.OK:
            if service == "Twitch":
                self.__streamMaster.setService(TwitchService())
            elif service == "YouTube":
                self.__streamMaster.setService(YouTubeService())
            else:
                self.__streamMaster.setService(FacebookService())
            
            self.log('Current service is ' + service)
            self.flashObject.updateControls()
            return True
        else:
            self.log('Failed to change service to ' + service)
            return False

    def onWindowSizeChanged(self):
        self.__smConnection.updateStreamResolution(self.getClientWindowWidth(), self.getClientWindowHeight())

    def getStreamState(self):
        return self.__smConnection.state

    def getStreamService(self):
        return self.__streamMaster.service.name

    def getStreamToken(self):
        return self.__streamMaster.service.token

    @staticmethod
    def getClientWindowWidth():
        if g_monitorSettings.isWindowed:
            return g_monitorSettings.currentWindowSize.width
        elif g_monitorSettings.isFullscreen:
            return g_monitorSettings.currentBorderlessSize.width

    @staticmethod
    def getClientWindowHeight():
        if g_monitorSettings.isWindowed:
            return g_monitorSettings.currentWindowSize.height
        elif g_monitorSettings.isFullscreen:
            return g_monitorSettings.currentBorderlessSize.height

    @staticmethod
    def log(message):
        MOD_LOG(message)

    # -- from view
    def checkIfTokenValid(self):

        isValid = False
        token = self.flashObject.token()
        self.log("Current token is: " + token)

        if token:
            matched = self.__streamMaster.service.pattern.match(token)
            if matched:
                self.__streamMaster.service.setToken(token)
                isValid = True

        return isValid
