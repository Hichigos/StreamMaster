#include "output.h"

Output::Output() {
	initializeDefaults();
	m_output = obs_output_create("rtmp_output", "simple_stream", m_settings, nullptr);
	obs_output_set_delay(m_output, 0, OBS_OUTPUT_DELAY_PRESERVE);
	obs_output_set_reconnect_settings(m_output, 20, 10);
	signalsConnect();
}

Output::~Output() {
	signalsDisconnect();
	obs_output_release(m_output);
}

void Output::initializeDefaults() {
	m_settings
		.SetString("bind_ip", "default")
		.SetBool("new_socket_loop_enabled", false)
		.SetBool("low_latency_mode_enabled", false);
}

void Output::signalsConnect() {
	signalConnect("starting", onOutputBusy);
	signalConnect("start", onOutputStarted);
	signalConnect("stopping", onOutputBusy);
	signalConnect("stop", onOutputStopped);
}

void Output::signalsDisconnect() {
	signalDisconnect("starting", onOutputBusy);
	signalDisconnect("start", onOutputStarted);
	signalDisconnect("stopping", onOutputBusy);
	signalDisconnect("stop", onOutputStopped);
}

void Output::signalConnect(const std::string &signal, signal_callback_t callback) {
	auto handler = obs_output_get_signal_handler(m_output);
	signal_handler_connect(handler, signal.c_str(), callback, this);
}

void Output::signalDisconnect(const std::string &signal, signal_callback_t callback) {
	auto handler = obs_output_get_signal_handler(m_output);
	signal_handler_disconnect(handler, signal.c_str(), callback, this);
}

void Output::setVideoEncoder(const EncoderPtr &encoder) {
	obs_output_set_video_encoder(m_output, *encoder);
}

void Output::setAudioEncoder(const EncoderPtr &encoder) {
	obs_output_set_audio_encoder(m_output, *encoder, 0);
}

void Output::setService(const ServicePtr &service) {
	obs_output_set_service(m_output, *service);
}

bool Output::updateSettings(const Settings &settings) {
	if (m_state == OutputState::Stopped) {
		m_settings = settings;
		obs_output_update(m_output, m_settings);
		return true;
	}
	return false;
}

bool Output::start() {
	utils::logAndPost("start output");

	if (m_state == OutputState::Stopped) {
		obs_output_start(m_output);
		return true;
	}
	return false;
}

bool Output::stop() {
	if (m_state == OutputState::Started) {
		obs_output_stop(m_output);
		return true;
	}
	return false;
}

OutputState Output::state() {
	return m_state;
}

void Output::setState(OutputState state) {
	m_state = state;
}


void onOutputStarted(void *data, calldata_t *params) {
	Output *output = static_cast<Output*>(data);
	output->setState(OutputState::Started);
}

void onOutputStopped(void *data, calldata_t *params) {
	Output *output = static_cast<Output*>(data);
	output->setState(OutputState::Stopped);
}

void onOutputBusy(void *data, calldata_t *params) {
	Output *output = static_cast<Output*>(data);
	output->setState(OutputState::Busy);
}