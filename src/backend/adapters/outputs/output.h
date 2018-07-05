/*
	Outputs allow the ability to output the currently rendering audio/video.
	Streaming and recording are two common examples of outputs, but not the only types of outputs.
	Outputs can receive the raw data or receive encoded data.
*/
#pragma once
#include "../utils/settings.h"
#include "../services/service.h"
#include "../encoders/encoder.h"

enum class OutputState {
	Stopped, Busy, Started
};

class Output {
public:
	Output();
	~Output();

	void setVideoEncoder(const EncoderPtr &encoder);
	void setAudioEncoder(const EncoderPtr &encoder);
	void setService(const ServicePtr &service);

	bool updateSettings(const Settings &settings);

	bool start();
	bool stop();

	OutputState state();
	void setState(OutputState state); // !! warning: public, church for obs callbacks

private:
	void initializeDefaults();

	void signalsConnect();
	void signalsDisconnect();

	void signalConnect(const std::string &signal, signal_callback_t callback);
	void signalDisconnect(const std::string &signal, signal_callback_t callback);

	Settings m_settings;
	obs_output* m_output = nullptr;

	std::atomic<OutputState> m_state = OutputState::Stopped;
};

static void onOutputStarted(void *data, calldata_t *params);
static void onOutputStopped(void *data, calldata_t *params);
static void onOutputBusy(void *data, calldata_t *params);