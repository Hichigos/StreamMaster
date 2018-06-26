#pragma once
#include "../../utils/shared.h"
#include "../../adapters/obs/outputs/output.h"

class IStreamClient {
public:
	virtual ~IStreamClient() {};

	virtual void UpdateStreamResolution(int width, int height) = 0;
	virtual void UpdateStreamToken(const std::string &token) = 0;

	virtual const OutputState GetStreamState() const = 0;

	virtual bool StartStream() = 0;
	virtual bool StopStream() = 0;
};

using IStreamClientPtr = std::shared_ptr<IStreamClient>;