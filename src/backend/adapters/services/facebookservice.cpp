#include "facebookservice.h"


FacebookService::FacebookService() {
	initializeDefaults();
	m_service = obs_service_create("rtmp_common", "default_service", m_settings, nullptr);
}

void FacebookService::initializeDefaults() {
	m_settings
		.SetString("server", "rtmp://live-api-s.facebook.com:80/rtmp/")
		.SetString("service", "Facebook Live");
}