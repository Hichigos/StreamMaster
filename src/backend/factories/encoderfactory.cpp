#include "encoderfactory.h"

#include "../adapters/obs/encoders/videoencoder.h"
#include "../adapters/obs/encoders/audioencoder.h"

EncoderPtr EncoderFactory::createAudioEncoder()
{
	return std::make_shared<AudioEncoder>();
}

EncoderPtr EncoderFactory::createVideoEncoder()
{
	return std::make_shared<AudioEncoder>();
}
