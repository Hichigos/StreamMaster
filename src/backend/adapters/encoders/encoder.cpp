#include "encoder.h"

Encoder::Encoder() {}

Encoder::~Encoder() {
	obs_encoder_release(encoder);
}

Encoder::operator obs_encoder* () const {
	return encoder;
}

obs_data* Encoder::GetSettings() const {
	return settings;
}

void Encoder::UpdateSettings(const Settings &settings) {
	this->settings = settings;
	obs_encoder_update(encoder, this->settings);
}
