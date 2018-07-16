#include "smappliation.h"

#include "../factories/streamclientfactory.h"
#include "../utils/constants.h"

using namespace std;

SMApplication::SMApplication() :
	m_streamClientPtr(std::move(StreamClientFactory::createStreamClient())),
	m_socket()
{
	m_socket.bindSocket(Default::Network::Address, Default::Network::Port);
	m_socket.listenSocket(Default::Network::MaxConnections);
}

void SMApplication::Run() {
	while (true)
	{
		m_socket.waitForNewConnection();

		while (m_socket.hasConnection()) {

			auto request = m_socket.receive(4);

			utils::logAndPost("Received message: " + request + "\n");

			if (request == Protocol::Request::Ping) {
				m_socket.sendData(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::State) {
				m_socket.sendData(streamState());
				
			} else if (request == Protocol::Request::Start) {
				m_socket.sendData(startStream());

			} else if (request == Protocol::Request::Stop) {
				m_socket.sendData(stopStream());

			} else if (request == Protocol::Request::UpdateToken) {
				m_socket.sendData(updateToken());

			} else if (request == Protocol::Request::UpdateService) {
				m_socket.sendData(updateStreamService());

			} else if (request == Protocol::Request::UpdateStreamRes) {
				m_socket.sendData(updateStreamResolution());
			}
		}
	}
}

const std::string SMApplication::streamState() const
{
	switch (m_streamClientPtr->streamState()) {
	case OutputState::Started:
		return Protocol::Replays::State::Started;

	case OutputState::Stopped:
		return Protocol::Replays::State::Stopped;

	case OutputState::Busy:
		return Protocol::Replays::State::Busy;
	}

	return Protocol::Replays::Network::Error;
}

const std::string SMApplication::startStream()
{
	if (m_streamClientPtr && m_streamClientPtr->startStream()) {
		int ticks = 0;
		while (m_streamClientPtr->streamState() != OutputState::Started && ticks++ < 500)
			this_thread::sleep_for(10ms);

		return m_streamClientPtr->streamState() == OutputState::Started ?
			Protocol::Replays::Network::OK :
			Protocol::Replays::Network::Error;
	}

	return Protocol::Replays::Network::Error;
}

const std::string SMApplication::stopStream()
{
	return m_streamClientPtr->stopStream() ? Protocol::Replays::Network::OK :
		Protocol::Replays::Network::Error;
}

const std::string SMApplication::updateToken()
{
	auto length = m_socket.receive(2);
	auto token = m_socket.receive(std::stoi(length));
	m_streamClientPtr->updateStreamToken(token);
	utils::logAndPost("Received token: " + token);

	return Protocol::Replays::Network::OK;
}

const std::string SMApplication::updateStreamService()
{
	auto length = m_socket.receive(1);
	auto service = m_socket.receive(std::stoi(length));
	utils::logAndPost("Received service: " + service);

	bool isUpdated = false;

	if (service == "Twitch")
		isUpdated = m_streamClientPtr->updateService(ServiceType::Twitch);
	else if (service == "YouTube")
		isUpdated = m_streamClientPtr->updateService(ServiceType::YouTube);
	else
		isUpdated = m_streamClientPtr->updateService(ServiceType::Facebook);

	return isUpdated ? Protocol::Replays::Network::OK : Protocol::Replays::Network::Error;
}

const std::string SMApplication::updateStreamResolution()
{
	auto length = m_socket.receive(1);
	auto resolution = m_socket.receive(std::stoi(length));

	utils::logAndPost("Received resolution: " + resolution);

	size_t pos = resolution.find('x');

	auto width = resolution.substr(0, pos);
	auto height = resolution.substr(pos + 1, resolution.length() - pos + 1);

	m_streamClientPtr->updateStreamResolution(std::stoi(width), std::stoi(height));

	return Protocol::Replays::Network::OK;
}

