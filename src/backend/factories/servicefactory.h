#pragma once

#include "../utils/shared.h"
#include "../adapters/services/service.h"

class ServiceFactory {
public:
	ServiceFactory() = delete;

	static ServicePtr createService(const ServiceType type);
};