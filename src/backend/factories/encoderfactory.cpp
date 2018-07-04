#include "encoderfactory.h"

#include "../adapters/encoders/videoencoder.h"
#include "../adapters/encoders/audioencoder.h"

EncoderPtr EncoderFactory::createAudioEncoder()
{
	return std::make_shared<AudioEncoder>();
}

EncoderPtr EncoderFactory::createVideoEncoder()
{
	return std::make_shared<VideoEncoder>();
}
