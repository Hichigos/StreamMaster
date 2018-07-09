#include "sourcefactory.h"

#include "../adapters/sources/audiocapturesource.h"
#include "../adapters/sources/videocapturesource.h"

SourcePtr SourceFactory::createSource(const SourceType type) {
	if (type == SourceType::Audio)
		return std::make_shared<AudioCaptureSource>();
	else if (type == SourceType::Video)
		return std::make_shared<VideoCaptureSource>();

	return nullptr;
}