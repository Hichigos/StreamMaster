/*
	Services are wrap implementations of streaming services, which are used with outputs that stream.
	For example, you could have a custom implementation for streaming to YouTube etc. to allow the ability
	to log in and use their APIs to do things such as get the RTMP servers or control the channel.
*/
#pragma once
#include "service.h"
#include "../encoders/encoder.h"


class TwitchService : public Service {
public:
	TwitchService();

protected:
	void InitializeDefaults();
};
