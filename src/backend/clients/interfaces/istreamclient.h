#pragma once

#include "../../utils/shared.h"
#include "../../adapters/outputs/output.h"
#include "../../adapters/services/service.h"

class IStreamClient {
public:
	virtual ~IStreamClient() {};

	virtual void updateStreamResolution(int width, int height) = 0;
	virtual void updateStreamToken(const std::string &token) = 0;

	virtual bool updateService(ServiceType type) = 0;

	virtual const OutputState streamState() const = 0;
	virtual const ServiceType serviceType() const = 0;

	virtual bool startStream() = 0;
	virtual bool stopStream() = 0;
};

using IStreamClientPtr = std::shared_ptr<IStreamClient>;