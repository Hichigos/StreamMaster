#pragma once

#include "interfaces/istreamclient.h"

#include "../adapters/encoders/encoder.h"
#include "../adapters/scene.h"
#include "../adapters/outputs/output.h"
#include "../adapters/services/service.h"


class OBSClient : public IStreamClient {
public:
	OBSClient();
	~OBSClient();

	void updateStreamResolution(int width, int height) override;
	void updateStreamToken(const std::string &token) override;
	bool updateService(ServiceType type) override;

	const OutputState streamState() const override;
	const ServiceType serviceType() const override;

	bool startStream() override;
	bool stopStream() override;

private:
	EncoderPtr m_videoEncoderPtr;
	EncoderPtr m_audioEncoderPtr;
	ServicePtr m_service;

	std::unique_ptr<Scene> m_scene;
	std::unique_ptr<Output> m_output;
};