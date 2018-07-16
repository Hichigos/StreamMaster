#include "socket.h"
#include "logger.h"

Socket::Socket() :
	m_socket(INVALID_SOCKET),
	m_addrinfo(nullptr),
	m_hasConnection(false)
{

}

Socket::Socket(SOCKET socket) {
	m_socket = socket;
}

Socket::~Socket() {
	closesocket(m_socket);
}

void Socket::bindSocket(const std::string &address, const std::string &port) {

	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) == 0) {
		m_addrinfo = resolveAddressInfo(address, port);
		createListenSocket();
		bindListeningSocket();
	}
}

void Socket::listenSocket(int max_connections) {
	if (listen(m_socket, max_connections) == SOCKET_ERROR) {
		onError(m_socket);
	}
}

std::string Socket::receive(int buffer_size) {

	std::string result = "";

	if (buffer_size > 0) {

		char* buffer = new char[buffer_size];

		int received = recv(m_socket, buffer, buffer_size, 0);
		if (received > 0) {
			result = std::string(buffer, received);
		}
		delete[] buffer;
	}
	return result;
}

void Socket::sendData(const std::string &data) {

	int sent = send(m_socket, data.c_str(), data.length(), 0);
	if (sent == SOCKET_ERROR) {
		onError(m_socket);
	}
	utils::logAndPost(data);
}

void Socket::waitForNewConnection()
{
	std::cout << "Wait for client connect" << std::endl;

	SOCKET accepted_socket = accept(m_socket, NULL, NULL);
	utils::logAndPost("Connection received");

	if (accepted_socket == INVALID_SOCKET) {
		onError(m_socket);
	}
	else {
		m_socket = accepted_socket;
		m_hasConnection = true;
		utils::logAndPost("Connection established");
	}	
}

const bool Socket::hasConnection() const
{
	return m_hasConnection;
}

addrinfo* Socket::resolveAddressInfo(const std::string &address, const std::string &port) {

	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	addrinfo* info;

	if (getaddrinfo(address.c_str(), port.c_str(), &hints, &info) == 0) {
		return info;
	}

	WSACleanup();
	return nullptr;
}

void Socket::createListenSocket() {
	m_socket = socket(m_addrinfo->ai_family, m_addrinfo->ai_socktype, m_addrinfo->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		utils::logAndPost(std::string("Error occured while createListenSocket() executed: " + std::to_string(WSAGetLastError())));
		onError(m_addrinfo);
	}
}

void Socket::bindListeningSocket() {
	if (bind(m_socket, m_addrinfo->ai_addr, (int)m_addrinfo->ai_addrlen) == SOCKET_ERROR) {
		utils::logAndPost("Error occured while socket bind() called with code: " + std::to_string(WSAGetLastError()));
		onError(m_addrinfo, m_socket);
	}
	freeaddrinfo(m_addrinfo);
}

void Socket::onError(addrinfo* address_info) {
	freeaddrinfo(address_info);
	address_info = nullptr;
	WSACleanup();
}

void Socket::onError(SOCKET socket) {
	utils::logAndPost("Connection refused with error: " + std::to_string(WSAGetLastError()));
	closesocket(socket);
	WSACleanup();
}

void Socket::onError(addrinfo* address_info, SOCKET socket) {
	freeaddrinfo(address_info);
	address_info = nullptr;
	closesocket(socket);
	WSACleanup();
}

WSAData Socket::m_wsaData;