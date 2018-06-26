#include "videoencoder.h"


VideoEncoder::VideoEncoder() {
	InitializeDefaults();
	encoder = obs_video_encoder_create("obs_x264", "simple_h264_stream", settings, nullptr);
	obs_encoder_set_video(encoder, obs_get_video());
}

void VideoEncoder::InitializeDefaults() {
	settings
		.SetString("rate_control", "CBR")
		.SetInt("bitrate", 2500);
}