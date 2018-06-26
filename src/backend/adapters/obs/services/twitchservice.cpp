#include "twitchservice.h"

TwitchService::TwitchService() {
	InitializeDefaults();
	service = obs_service_create("rtmp_common", "default_service", settings, nullptr);
}

void TwitchService::InitializeDefaults() {
	settings
		.SetString("server", "rtmp://live-waw.twitch.tv/app/") // + token
		.SetString("service", "Twitch");
}