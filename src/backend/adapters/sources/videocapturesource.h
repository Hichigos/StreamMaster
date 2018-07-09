/*
	Sources are used to render video and/or audio on stream.
	Things such as capturing displays/games/audio, playing a video,
	showing an image, or playing audio.
*/
#pragma once
#include "source.h"


class VideoCaptureSource : public Source {
public:
	VideoCaptureSource();

protected:
	void initializeDefaults();
};