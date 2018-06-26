/*
	Services are wrap implementations of streaming services, which are used with outputs that stream.
	For example, you could have a custom implementation for streaming to YouTube etc. to allow the ability
	to log in and use their APIs to do things such as get the RTMP servers or control the channel.
*/
#pragma once

#include <memory>

#include "../utils/settings.h"
#include "../encoders/encoder.h"

class Service {
public:
	virtual ~Service();
	operator obs_service* () const;

	void ApplyEncoders(const EncoderPtr &video_encoder, const EncoderPtr &audio_encoder);
	void UpdateToken(const std::string &token);
	void UpdateSettings(const Settings &settings);

	const char* GetOutputType();

protected:
	Service();

	Settings settings;
	obs_service* service;
	char* output_type;
};

using ServicePtr = std::shared_ptr<Service>;