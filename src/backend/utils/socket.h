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

	void Bind(const std::string &address, const std::string &port);
	void Listen(int max_connections);

	std::string Receive(int buffer_size);
	void Send(const std::string &data);

	void waitForNewConnection();

	const bool hasConnection() const;

private:
	addrinfo * ResolveAddressInfo(const std::string &address, const std::string &port);
	void CreateListenSocket();
	void BindListeningSocket();

	void OnError(addrinfo* address_info);
	void OnError(SOCKET socket);
	void OnError(addrinfo* address_info, SOCKET socket);

private:
	static WSAData wsa_data;

	addrinfo* m_addrinfo;
	SOCKET m_socket;

	bool m_hasConnection;
};