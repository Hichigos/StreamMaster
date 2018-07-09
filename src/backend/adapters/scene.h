#pragma once
#include "../utils/shared.h"
#include "sources/source.h"
#include "utils\constants.h"

class Scene {
public:
	Scene(int width = OBSDefault::Scene::Width, int height = OBSDefault::Scene::Height);
	~Scene();

	operator obs_scene*() const;

	void updateStreamResolution(int width, int height);

private:
	void resetAudio(speaker_layout layout, int sps);
	void resetVideo(int width, int height, int fps);

	obs_scene* m_scene;

	SourcePtr m_videoSource;
	SourcePtr m_audioSource;
};