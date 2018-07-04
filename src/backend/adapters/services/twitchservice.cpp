#include "twitchservice.h"

TwitchService::TwitchService() {
	InitializeDefaults();
	m_service = obs_service_create("rtmp_common", "default_service", m_settings, nullptr);
}

void TwitchService::InitializeDefaults() {
	m_settings
		.SetString("server", "rtmp://live-waw.twitch.tv/app") // + token live_235276298_e7CntkmMufzzCR2A12DQmBWEZXZhNI
		.SetString("service", "Twitch");
}