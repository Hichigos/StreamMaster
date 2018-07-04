#include "service.h"

Service::Service() :
	m_service(nullptr),
	m_serviceType(ServiceType::None) 
{}

Service::~Service() { 
	obs_service_release(m_service); 
}

Service::operator obs_service*() const { 
	return m_service;
}

void Service::ApplyEncoders(const EncoderPtr &video_encoder, const EncoderPtr &audio_encoder) {
	obs_service_apply_encoder_settings(m_service, video_encoder->GetSettings(), audio_encoder->GetSettings());
}

void Service::UpdateToken(const std::string &token) {
	m_settings
		.SetString("key", token.c_str());

	obs_service_update(m_service, m_settings);
	utils::log_string("New token: " + token);
}

void Service::UpdateSettings(const Settings &settings) {
	m_settings = settings;
	obs_service_update(m_service, m_settings);
}

const ServiceType Service::serviceType() const
{
	return m_serviceType;
}

const char* Service::GetOutputType() {
	return obs_service_get_output_type(m_service);
}
