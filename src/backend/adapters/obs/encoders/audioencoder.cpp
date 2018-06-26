#include "audioencoder.h"

AudioEncoder::AudioEncoder() {
	InitializeDefaults();
	encoder = obs_audio_encoder_create("mf_aac", "simple_aac", settings, 0, nullptr);
	obs_encoder_set_audio(encoder, obs_get_audio());
}

void AudioEncoder::InitializeDefaults() {
	settings
		.SetString("rate_control", "CBR")
		.SetInt("bitrate", 160);
}