#include "encoder.h"

Encoder::Encoder() {}

Encoder::~Encoder() {
	obs_encoder_release(m_encoder);
}

Encoder::operator obs_encoder* () const {
	return m_encoder;
}

obs_data* Encoder::settings() const {
	return m_settings;
}

void Encoder::updateSettings(const Settings &settings) {
	m_settings = settings;
	obs_encoder_update(m_encoder, m_settings);
}
