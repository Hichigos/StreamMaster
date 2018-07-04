import string

from gui.Scaleform.framework import g_entitiesFactories, ViewSettings
from gui.Scaleform.framework import ViewTypes, ScopeTemplates
from gui.Scaleform.framework.managers.loaders import SFViewLoadParams
from gui.app_loader import g_appLoader
from gui.shared.utils.key_mapping import getBigworldNameFromKey
from gui import InputHandler

from mod_overlaywindow import OverlayWindow


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
