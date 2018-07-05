#include "audioencoder.h"

AudioEncoder::AudioEncoder() {
	initializeDefaults();
	m_encoder = obs_audio_encoder_create("mf_aac", "simple_aac", m_settings, 0, nullptr);
	obs_encoder_set_audio(m_encoder, obs_get_audio());
}

void AudioEncoder::initializeDefaults() {
	m_settings
		.SetString("rate_control", "CBR")
		.SetInt("bitrate", 160);
}