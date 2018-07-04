#include "scene.h"

#include "sources\audiocapturesource.h"
#include "sources\videocapturesource.h"


using namespace std;

Scene::Scene(int width, int height) {

	ResetAudio(SPEAKERS_STEREO, 44100);
	ResetVideo(width, height, 60);

	scene = obs_scene_create("scene");

	m_videoSource = std::make_unique<VideoCaptureSource>();
	m_audioSource = std::make_unique<AudioCaptureSource>();

	obs_set_output_source(0, *m_videoSource);
	obs_set_output_source(1, *m_audioSource);
}

Scene::~Scene() { obs_scene_release(scene); }

Scene::operator obs_scene* () const { return scene; }

void Scene::UpdateScreenSize(int width, int height) {
	ResetVideo(width, height, 60);
}

void Scene::ResetAudio(speaker_layout layout, int sps) {

	auto ai = std::make_shared<obs_audio_info>();

	ai->samples_per_sec = sps;
	ai->speakers = layout;

	if (!obs_reset_audio(ai.get())) {
		throw "audio: reset failed";
	}
}

void Scene::ResetVideo(int width, int height, int fps) {

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