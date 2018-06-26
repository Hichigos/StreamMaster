#pragma once

#include "interfaces/istreamclient.h"

#include "../adapters/obs/encoders/encoder.h"
#include "../adapters/obs/scene.h"
#include "../adapters/obs/outputs/output.h"
#include "../adapters/obs/services/service.h"


class OBSClient : public IStreamClient {
public:
	OBSClient();
	~OBSClient();

	virtual void UpdateStreamResolution(int width, int height) override;
	virtual void UpdateStreamToken(const std::string &token) override;

	virtual const OutputState GetStreamState() const override;

	virtual bool StartStream() override;
	virtual bool StopStream() override;

private:
	EncoderPtr m_videoEncoderPtr;
	EncoderPtr m_audioEncoderPtr;
	ServicePtr m_service;

	std::unique_ptr<Scene> m_scene;
	std::unique_ptr<Output> m_output;
};