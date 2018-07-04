#pragma once

#include "../utils/shared.h"
#include "../adapters/encoders/encoder.h"

class EncoderFactory {
public:
	EncoderFactory() = delete;

	static EncoderPtr createAudioEncoder();
	static EncoderPtr createVideoEncoder();
};