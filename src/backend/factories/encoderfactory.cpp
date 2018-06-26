#include "encoderfactory.h"

#include "../adapters/obs/encoders/videoencoder.h"
#include "../adapters/obs/encoders/audioencoder.h"

IEncoderPtr EncoderFactory::createAudioEncoder()
{
	return std::make_shared<AudioEncoder>();
}

IEncoderPtr EncoderFactory::createVideoEncoder()
{
	return std::make_shared<AudioEncoder>();
}
