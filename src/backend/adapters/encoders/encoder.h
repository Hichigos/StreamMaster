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

	obs_data* settings() const;
	void updateSettings(const Settings &settings);

protected:
	Encoder();

	Settings m_settings;
	obs_encoder* m_encoder;
};

using EncoderPtr = std::shared_ptr<Encoder>;