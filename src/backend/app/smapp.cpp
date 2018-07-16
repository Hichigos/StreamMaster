#include "smapp.h"
#include "../factories/streamclientfactory.h"
#include "../utils/constants.h"

using namespace std;

SMApp::SMApp() :
	m_streamClientPtr(std::move(StreamClientFactory::createStreamClient())),
	m_socket()
{
	m_socket.bindSocket(Default::Network::Address, Default::Network::Port);
	m_socket.listenSocket(Default::Network::MaxConnections);
}

void SMApp::Run() {
	while (true)
	{
		m_socket.waitForNewConnection();

		while (m_socket.hasConnection()) {

			auto request = m_socket.receive(4);

			utils::log_string("Received message: " + request + "\n");

			if (request == Protocol::Request::Ping) {
				m_socket.sendData(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::State) {

				switch (m_streamClientPtr->streamState()) {
					case OutputState::Started:
						m_socket.sendData(Protocol::Replays::State::Started);
						break;

					case OutputState::Stopped:
						m_socket.sendData(Protocol::Replays::State::Stopped);
						break;

					case OutputState::Busy:
						m_socket.sendData(Protocol::Replays::State::Busy);
						break;
				}
				
			} else if (request == Protocol::Request::Start) {

				if (m_streamClientPtr && m_streamClientPtr->startStream()) {
					int ticks = 0;
					while (m_streamClientPtr->streamState() != OutputState::Started && ticks++ < 500)
						this_thread::sleep_for(10ms);

					m_socket.sendData(m_streamClientPtr->streamState() == OutputState::Started ?
						Protocol::Replays::Network::OK :
						Protocol::Replays::Network::Error);
				}
				else {
					m_socket.sendData(Protocol::Replays::Network::BadOperation);
				}

			} else if (request == Protocol::Request::Stop) {
				m_streamClientPtr->stopStream();
				m_socket.sendData(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::UpdateToken) {
				auto length = m_socket.receive(2);
				auto token = m_socket.receive(std::stoi(length));
				m_streamClientPtr->updateStreamToken(token);
				utils::log_string("Received token: " + token);
				m_socket.sendData(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::UpdateService) {
				auto length = m_socket.receive(1);
				auto service = m_socket.receive(std::stoi(length));
				utils::log_string("Received service: " + service);

				if (service == "Twitch")
					m_streamClientPtr->updateService(ServiceType::Twitch);
				else if (service == "YouTube")
					m_streamClientPtr->updateService(ServiceType::YouTube);
				else
					m_streamClientPtr->updateService(ServiceType::Facebook);

				m_socket.sendData(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::UpdateStreamRes) {
				auto length = m_socket.receive(1);
				auto resolution = m_socket.receive(std::stoi(length));

				utils::log_string("Received resolution: " + resolution);

				size_t pos = resolution.find('x');

				auto width = resolution.substr(0, pos);
				utils::log_string("Width: " + width);

				auto height = resolution.substr(pos + 1, resolution.length() - pos + 1);
				utils::log_string("Height: " + height);

				m_streamClientPtr->updateStreamResolution(std::stoi(width), std::stoi(height));
				m_socket.sendData(Protocol::Replays::Network::OK);
			}

			std::this_thread::sleep_for(10ms);
		}
	}
}

