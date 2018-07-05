/*
	Utility adapter over WinSock
*/

#pragma once
#include "shared.h"

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

class Socket {
public:
	Socket();
	Socket(SOCKET socket);
	~Socket();

	void bindSocket(const std::string &address, const std::string &port);
	void listenSocket(int max_connections);

	std::string receive(int buffer_size);
	void sendData(const std::string &data);

	void waitForNewConnection();

	const bool hasConnection() const;

private:
	addrinfo * resolveAddressInfo(const std::string &address, const std::string &port);
	void createListenSocket();
	void bindListeningSocket();

	void onError(addrinfo* address_info);
	void onError(SOCKET socket);
	void onError(addrinfo* address_info, SOCKET socket);

private:
	static WSAData m_wsaData;

	addrinfo* m_addrinfo;
	SOCKET m_socket;

	bool m_hasConnection;
};