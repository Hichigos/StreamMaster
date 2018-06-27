#include "obsclient.h"

#include "../factories/encoderfactory.h"
#include "../factories/servicefactory.h"

#include "../adapters/obs/utils/modules_loader.h"
#include "../utils/shared.h"

#include <utility>


OBSClient::OBSClient()
{
	if (!obs_startup("en-US", nullptr, nullptr)) {
		throw "OBS startup failed";
	}

	auto modules_loader = std::make_unique<ModulesLoader>();
	modules_loader->LoadAuthorized();

	initialize();

	m_service->ApplyEncoders(m_videoEncoderPtr, m_audioEncoderPtr);

	m_output->SetVideoEncoder(m_videoEncoderPtr);
	m_output->SetAudioEncoder(m_audioEncoderPtr);
	m_output->SetService(m_service);
}

OBSClient::~OBSClient()
{
	while (m_output && m_output->GetState() != OutputState::Stopped) {
		m_output->Stop();
		//std::this_thread::sleep_for();
	}
	m_output.release();
	m_service.reset();
	m_audioEncoderPtr.reset();
	m_videoEncoderPtr.reset();
	m_scene.release();
	obs_shutdown();
}

void OBSClient::UpdateStreamResolution(int width, int height) {
	m_scene->UpdateScreenSize(width, height);
}

void OBSClient::UpdateStreamToken(const std::string & token) {
	m_service->UpdateToken(token);
}

const OutputState OBSClient::GetStreamState() const {
	return m_output->GetState();
}

bool OBSClient::StartStream() {
	return m_output->Start();
}

bool OBSClient::StopStream() {
	return m_output->Stop();
}

void OBSClient::initialize() {
	m_videoEncoderPtr = std::move(EncoderFactory::createVideoEncoder());
	m_audioEncoderPtr = std::move(EncoderFactory::createAudioEncoder());
	m_scene = std::move(std::make_unique<Scene>());
	m_service = std::move(ServiceFactory::createYouTubeService());
	m_output = std::move(std::make_unique<Output>());

	utils::log_string("OBS client initialized\n");
}
