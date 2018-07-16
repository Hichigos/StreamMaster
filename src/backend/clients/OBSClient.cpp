#include "obsclient.h"

#include "../factories/encoderfactory.h"
#include "../factories/servicefactory.h"

#include "../adapters/utils/modules_loader.h"
#include "../utils/shared.h"

#include <utility>

using namespace std;

OBSClient::OBSClient()
{
	if (!obs_startup("en-US", nullptr, nullptr)) {
		throw "OBS startup failed";
	}

	auto modules_loader = std::make_unique<ModulesLoader>();
	modules_loader->LoadAuthorized();

	m_scene = std::make_unique<Scene>();

	m_videoEncoderPtr = EncoderFactory::createVideoEncoder();
	m_audioEncoderPtr = EncoderFactory::createAudioEncoder();

	m_service = nullptr;

	m_output = std::make_unique<Output>();
	m_output->setVideoEncoder(m_videoEncoderPtr);
	m_output->setAudioEncoder(m_audioEncoderPtr);

	utils::log_string("obs client initialized");
}

OBSClient::~OBSClient()
{
	while (m_output && m_output->state() != OutputState::Stopped) {
		m_output->stop();
		std::this_thread::sleep_for(200ms);
	}
	m_output.release();
	m_service.reset();
	m_audioEncoderPtr.reset();
	m_videoEncoderPtr.reset();
	m_scene.release();
	obs_shutdown();
}

void OBSClient::updateStreamResolution(int width, int height) {
	m_scene->updateStreamResolution(width, height);
}

void OBSClient::updateStreamToken(const std::string & token) {
	m_service->updateToken(token);
	
}

bool OBSClient::updateService(ServiceType type) {
	if (type == ServiceType::None)
		return false;

	if (m_service && m_service->serviceType() != type) {
		m_service.reset();
	}

	m_service = ServiceFactory::createService(type);

	m_service->applyEncoders(m_videoEncoderPtr, m_audioEncoderPtr);
	m_output->setService(m_service);
	return true;
}

const OutputState OBSClient::streamState() const {
	return m_output->state();
}

const ServiceType OBSClient::serviceType() const
{
	return m_service->serviceType();
}

bool OBSClient::startStream() {
	return m_output->start();
}

bool OBSClient::stopStream() {
	return m_output->stop();
}
