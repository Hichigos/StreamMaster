#include "videocapturesource.h"


VideoCaptureSource::VideoCaptureSource() {
	InitializeDefaults();
	source = obs_source_create("game_capture", "World of Tanks", settings, nullptr);
}

void VideoCaptureSource::InitializeDefaults() {
	settings
		.SetString("capture_mode", "window")
		.SetString("window", "WoT Client:App:WorldOfTanks.exe")
		.SetInt("priority", 2)
		.SetBool("sli_compatibility", false)
		.SetBool("capture_cursor", true)
		.SetBool("allow_transparency", false)
		.SetBool("force_scaling", false)
		.SetBool("limit_framerate", false)
		.SetBool("capture_overlays", false)
		.SetBool("anti_cheat_hook", true)
		.SetString("scale_res", "1600x900");
	settings
		.SetDefaultDouble("volume", 1.0)
		.SetDefaultInt("mixers", 0xF)
		.SetDefaultInt("flags", 0)
		.SetDefaultBool("enabled", true);
}