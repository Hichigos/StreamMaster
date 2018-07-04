#include "youtubeservice.h"


YouTubeService::YouTubeService() {
	InitializeDefaults();
	m_service = obs_service_create("rtmp_common", "default_service", m_settings, nullptr);
}

void YouTubeService::InitializeDefaults() {
	m_settings
		.SetString("server", "rtmp://a.rtmp.youtube.com/live2")
		.SetString("service", "YouTube / YouTube Gaming");
}