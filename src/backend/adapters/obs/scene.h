#pragma once
#include "../../utils/shared.h"
#include "sources/source.h"

class Scene {
public:
	Scene(int width = 1920, int height = 1080);
	~Scene();

	operator obs_scene*() const;

	void UpdateScreenSize(int width, int height);

private:
	void ResetAudio(speaker_layout layout, int sps);
	void ResetVideo(int width, int height, int fps);

	obs_scene* scene;

	SourcePtr m_videoSource;
	SourcePtr m_audioSource;
};