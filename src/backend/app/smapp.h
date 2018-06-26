#include "../clients/interfaces/istreamclient.h"
#include "../utils/socket.h"

class SMApp {
public:
	SMApp();
	void Run();

private:
	IStreamClientPtr m_streamClientPtr;
	Socket m_socket;
};