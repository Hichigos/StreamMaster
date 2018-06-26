#pragma once

#include "../utils/shared.h"
#include "../adapters/obs/encoders/encoder.h"

class EncoderFactory {
public:

	static EncoderPtr createAudioEncoder();
	static EncoderPtr createVideoEncoder();
};