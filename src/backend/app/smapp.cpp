#include "smapp.h"
#include "../clients/obsclient.h"
#include "../utils/constants.h"

using namespace std;

SMApp::SMApp() :
	m_streamClientPtr(std::move(std::make_shared<OBSClient>())),
	m_socket()
{
	m_socket.Bind(Default::Network::Address, Default::Network::Port);
	m_socket.Listen(Default::Network::MaxConnections);
}

void SMApp::Run() {
	while (true)
	{
		m_socket.waitForNewConnection();

		while (m_socket.hasConnection()) {

			auto request = m_socket.Receive(4);

			//if (request.length() > 0)
				utils::log_string("Received message: " + request + "\n");

			if (request == Protocol::Request::Ping) {
				m_socket.Send(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::State) {

				switch (m_streamClientPtr->streamState()) {
					case OutputState::Started:
						m_socket.Send(Protocol::Replays::State::Started);
						break;

					case OutputState::Stopped:
						m_socket.Send(Protocol::Replays::State::Stopped);
						break;

					case OutputState::Busy:
						m_socket.Send(Protocol::Replays::State::Busy);
						break;
				}
				
			} else if (request == Protocol::Request::Start) {
				m_streamClientPtr->startStream();
				m_socket.Send(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::Stop) {
				m_streamClientPtr->stopStream();
				m_socket.Send(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::UpdateToken) {
				auto length = m_socket.Receive(2);
				auto token = m_socket.Receive(std::stoi(length));
				m_streamClientPtr->updateStreamToken(token); //"live_235276298_e7CntkmMufzzCR2A12DQmBWEZXZhNI"
				utils::log_string("Received message: " + token);
				m_socket.Send(Protocol::Replays::Network::OK);

			} else if (request == Protocol::Request::UpdateService) {
				auto length = m_socket.Receive(1);
				auto service = m_socket.Receive(std::stoi(length));
				utils::log_string("Received message: " + service);

				if (service == "Twitch")
					m_streamClientPtr->updateService(ServiceType::Twitch);
				else
					m_streamClientPtr->updateService(ServiceType::YouTube);

				m_socket.Send(Protocol::Replays::Network::OK);
			}

			std::this_thread::sleep_for(10ms);
		}
	}
}

