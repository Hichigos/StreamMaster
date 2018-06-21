from gui.Scaleform.framework import g_entitiesFactories, ViewSettings
from gui.Scaleform.framework import ViewTypes, ScopeTemplates
from gui.Scaleform.framework.managers.loaders import SFViewLoadParams
from gui.app_loader import g_appLoader
from gui.shared.utils.key_mapping import getBigworldNameFromKey

from gui import InputHandler

from gui.shared.utils.graphics import g_monitorSettings
from gui.Scaleform.framework.entities.abstract.AbstractWindowView import AbstractWindowView
from gui import SystemMessages
import debug_utils

class OverlayWindow(AbstractWindowView):

    def __init__(self):
        AbstractWindowView.__init__(self)

    def _populate(self):
        AbstractWindowView._populate(self)

    def _dispose(self):
        AbstractWindowView._dispose(self)

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
