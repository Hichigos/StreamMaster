#include "videoencoder.h"


VideoEncoder::VideoEncoder() {
	initializeDefaults();
	m_encoder = obs_video_encoder_create("obs_x264", "simple_h264_stream", m_settings, nullptr);
	obs_encoder_set_video(m_encoder, obs_get_video());
}

void VideoEncoder::initializeDefaults() {
	m_settings
		.SetString("rate_control", "CBR")
		.SetInt("bitrate", 2500);
}