import re

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
        self._pattern = re.compile(r'live_\d{9}_\w{30}', re.IGNORECASE)


class FacebookService(StreamService):
    def __init__(self):
        super(FacebookService, self).__init__()
        self._name = "Facebook"
        self._pattern = re.compile(r'\d{15,16}\?s_ps=1&s_vt=api-s&a=[\w-]{16}', re.IGNORECASE)