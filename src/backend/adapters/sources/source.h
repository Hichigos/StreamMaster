/*
	Sources are used to render video and/or audio on stream.
	Things such as capturing displays/games/audio, playing a video,
	showing an image, or playing audio.
*/
#pragma once
#include "../utils/settings.h"
#include "../../utils/shared.h"

enum SourceType {
	Base = 0,
	Audio,
	Video
};

class Source {
public:
	virtual ~Source();
	operator obs_source*() const;

	void updateSettings(const Settings &settings);
	void updateScaleResolution(int width, int height);

protected:
	Source();

	Settings m_settings;
	obs_source* m_source;
};

using SourcePtr = std::shared_ptr<Source>;