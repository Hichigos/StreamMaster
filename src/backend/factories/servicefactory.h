#pragma once

#include "../utils/shared.h"
#include "../adapters/obs/services/service.h"

class ServiceFactory {
public:
	ServiceFactory() = delete;

	static ServicePtr createYouTubeService();
	static ServicePtr createTwitchService();
};