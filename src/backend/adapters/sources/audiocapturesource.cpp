#include "audiocapturesource.h"


AudioCaptureSource::AudioCaptureSource() {
	InitializeDefaults();
	source = obs_source_create("wasapi_output_capture", "Desktop Audio", settings, nullptr);
}

void AudioCaptureSource::InitializeDefaults() {
	settings
		.SetDefaultDouble("volume", 1.0)
		.SetDefaultInt("mixers", 0xFF)
		.SetDefaultInt("flags", 0)
		.SetDefaultBool("enabled", true)
		.SetDefaultBool("muted", false);
}