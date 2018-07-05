#include "twitchservice.h"

TwitchService::TwitchService() {
	InitializeDefaults();
	m_service = obs_service_create("rtmp_common", "default_service", m_settings, nullptr);
}

void TwitchService::InitializeDefaults() {
	m_settings
		.SetString("server", "rtmp://live-waw.twitch.tv/app")
		.SetString("service", "Twitch");
}