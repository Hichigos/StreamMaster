#include "scene.h"

#include "../factories/sourcefactory.h"


using namespace std;

Scene::Scene(int width, int height) {

	resetAudio(SPEAKERS_STEREO, 44100);
	resetVideo(width, height, 60);

	m_scene = obs_scene_create("scene");

	m_videoSource = SourceFactory::createSource(SourceType::Video);
	m_audioSource = SourceFactory::createSource(SourceType::Audio);

	obs_set_output_source(0, *m_videoSource);
	obs_set_output_source(1, *m_audioSource);
}

Scene::~Scene() { obs_scene_release(m_scene); }

Scene::operator obs_scene* () const { return m_scene; }

void Scene::updateStreamResolution(int width, int height) {
	//resetVideo(width, height, 60);
	m_videoSource->updateScaleResolution(width, height);
}

void Scene::resetAudio(speaker_layout layout, int sps) {

	auto ai = std::make_shared<obs_audio_info>();

	ai->samples_per_sec = sps;
	ai->speakers = layout;

	if (!obs_reset_audio(ai.get())) {
		throw "audio: reset failed";
	}
}

void Scene::resetVideo(int width, int height, int fps) {

	auto vi = std::make_shared<obs_video_info>();

	vi->fps_num = fps;
	vi->fps_den = 1;
	vi->graphics_module = "libobs-d3d11.dll";
	vi->base_width = width;
	vi->base_height = height;
	vi->output_width = vi->base_width;
	vi->output_height = vi->base_height;
	vi->output_format = VIDEO_FORMAT_NV12;
	vi->colorspace = VIDEO_CS_601;
	vi->range = VIDEO_RANGE_FULL;
	vi->adapter = 0;
	vi->gpu_conversion = true;
	vi->scale_type = OBS_SCALE_BICUBIC;

	auto code = obs_reset_video(vi.get());

	if (code != OBS_VIDEO_SUCCESS) {
		std::string message = "video: reset failed";
		throw (message += ", error code: " + code).c_str();
	}
}