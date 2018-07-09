#include "audiocapturesource.h"


AudioCaptureSource::AudioCaptureSource() {
	initializeDefaults();
	m_source = obs_source_create("wasapi_output_capture", "Desktop Audio", m_settings, nullptr);
}

void AudioCaptureSource::initializeDefaults() {
	m_settings
		.SetDefaultDouble("volume", 1.0)
		.SetDefaultInt("mixers", 0xFF)
		.SetDefaultInt("flags", 0)
		.SetDefaultBool("enabled", true)
		.SetDefaultBool("muted", false);
}