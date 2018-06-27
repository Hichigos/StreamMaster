#include "smapp.h"
#include "../clients/obsclient.h"


SMApp::SMApp() :
	m_streamClientPtr(std::move(std::make_shared<OBSClient>())),
	m_socket()
{
	m_socket.Bind("127.0.0.1", "48684");
	m_socket.Listen(1);
}

void SMApp::Run() {
	while (true)
	{
		m_socket.waitForNewConnection();

		while (m_socket.hasConnection()) {

			auto request = m_socket.Receive(4);

			utils::log_string("Received message: " + request + "\n");

			if (request == "stat") {
				m_socket.Send("ok");
			}
			else if (request == "stst") {
				m_streamClientPtr->StartStream();
				m_socket.Send("ok");
			}
			else if (request == "spst") {
				m_streamClientPtr->StopStream();
				m_socket.Send("ok");
			}
			else if (request == "updt") {
				m_socket.Receive(2);
				request = m_socket.Receive(19);
				m_streamClientPtr->UpdateStreamToken(request);
				m_socket.Send("ok");
			}
			else if (request == "init") {
				m_socket.Send("ok");
			}
		}
	}
}

