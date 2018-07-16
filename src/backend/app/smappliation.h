#pragma once

#include "../clients/interfaces/istreamclient.h"
#include "../utils/socket.h"

class SMApplication {
public:
	SMApplication();

	void Run();

private:
	const std::string streamState() const;

	const std::string startStream();

	const std::string stopStream();

	const std::string updateToken();

	const std::string updateStreamService();

	const std::string updateStreamResolution();

private:
	IStreamClientPtr m_streamClientPtr;
	Socket m_socket;
};