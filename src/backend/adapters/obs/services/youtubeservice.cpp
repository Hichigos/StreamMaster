#include "youtubeservice.h"


YouTubeService::YouTubeService() {
	InitializeDefaults();
	service = obs_service_create("rtmp_common", "default_service", settings, nullptr);
}

void YouTubeService::InitializeDefaults() {
	settings
		.SetString("server", "rtmp://a.rtmp.youtube.com/live2")
		.SetString("service", "YouTube / YouTube Gaming");
}