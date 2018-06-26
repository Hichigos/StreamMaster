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

			//if (request == "stat") {
			//	connection->Send(WSGetState());
			//}
			//else if (request == "srts") {
			//	connection->Send(WSStartStream());
			//}
			//else if (request == "stps") {
			//	connection->Send(WSStopStream());
			//}
			//else if (request == "updt") {
			//	connection->Receive(2);
			//	request = connection->Receive(19);
			//	connection->Send(WSUpdateToken(request));
			//}
			//else if (request == "init") {
			//	connection->Send(WSInitialize());
			//}
			//else {
			//	connection->Send("unauthorized request");
			//	break;
			//}
		}
	}
}

