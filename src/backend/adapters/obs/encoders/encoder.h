/*
	Encoders are specific implementations of video/audio encoders,
	which are used with outputs that use encoders.
	x264, NVENC, Quicksync are examples of encoder implementations.
*/
#pragma once
#include "../utils/settings.h"

class Encoder {
public:
	virtual ~Encoder();
	operator obs_encoder* () const;

	obs_data* GetSettings() const;
	void UpdateSettings(const Settings &settings);

protected:
	Encoder();

	Settings settings;
	obs_encoder* encoder;
};

using EncoderPtr = std::shared_ptr<Encoder>;