/*
	Sources are used to render video and/or audio on stream.
	Things such as capturing displays/games/audio, playing a video,
	showing an image, or playing audio.
*/
#pragma once
#include "../utils/settings.h"
#include "../../../utils/shared.h"


class Source {
public:
	virtual ~Source();
	operator obs_source*() const;

	void UpdateSettings(const Settings &settings);

protected:
	Source();

	Settings settings;
	obs_source* source;
};

using SourcePtr = std::shared_ptr<Source>;