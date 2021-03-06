/*
    Encoders are specific implementations of video/audio encoders,
    which are used with outputs that use encoders.
    x264, NVENC, Quicksync are examples of encoder implementations.
*/
#pragma once
#include "encoder.h"

class VideoEncoder : public Encoder {
public:
	VideoEncoder();

private:
	void initializeDefaults();
};

