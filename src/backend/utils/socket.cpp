#include "socket.h"
#include "logger.h"

Socket::Socket() :
	m_socket(INVALID_SOCKET),
	m_addrinfo(nullptr),
	m_hasConnection(false)
{

}

Socket::Socket(SOCKET socket) {
	this->m_socket = socket;
}

Socket::~Socket() {
	closesocket(m_socket);
}

void Socket::Bind(const std::string &address, const std::string &port) {

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0) {
		m_addrinfo = ResolveAddressInfo(address, port);
		CreateListenSocket();
		BindListeningSocket();
	}
}

void Socket::Listen(int max_connections) {
	utils::log_string("Listen \n");
	if (listen(m_socket, max_connections) == SOCKET_ERROR) {
		OnError(m_socket);
	}
}

std::string Socket::Receive(int buffer_size) {

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

void Socket::Send(const std::string &data) {

	int sent = send(m_socket, data.c_str(), data.length(), 0);
	if (sent == SOCKET_ERROR) {
		OnError(m_socket);
	}
	utils::log_string(data);
}

void Socket::waitForNewConnection()
{
	std::cout << "Wait for client connect" << std::endl;

	SOCKET accepted_socket = accept(m_socket, NULL, NULL);

	if (accepted_socket == INVALID_SOCKET) {
		OnError(m_socket);
	}
	else {
		m_socket = accepted_socket;
		utils::log_string("Connection received");
		m_hasConnection = true;
		std::cout << "Connection established" << std::endl;
	}	
}

const bool Socket::hasConnection() const
{
	return m_hasConnection;
}

// HARDCODE, fake resolve
addrinfo* Socket::ResolveAddressInfo(const std::string &address, const std::string &port) {

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

void Socket::CreateListenSocket() {
	utils::log_string("CreateListenSocket \n");

	m_socket = socket(m_addrinfo->ai_family, m_addrinfo->ai_socktype, m_addrinfo->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		utils::log_string(std::string("Error occured while CreateListenSocket() executed: " + std::to_string(WSAGetLastError())));
		OnError(m_addrinfo);
	}
}

void Socket::BindListeningSocket() {
	utils::log_string("BindListeningSocket\n");
	if (bind(m_socket, m_addrinfo->ai_addr, (int)m_addrinfo->ai_addrlen) == SOCKET_ERROR) {
		utils::log_string("Error occured while socket bind() called with code: " + std::to_string(WSAGetLastError()));
		OnError(m_addrinfo, m_socket);
	}
	freeaddrinfo(m_addrinfo);
}

void Socket::OnError(addrinfo* address_info) {
	freeaddrinfo(address_info);
	address_info = nullptr;
	WSACleanup();
}

void Socket::OnError(SOCKET socket) {
	utils::log_string("Connection refused with error: " + std::to_string(WSAGetLastError()) + "\n");
	closesocket(socket);
	WSACleanup();
}

void Socket::OnError(addrinfo* address_info, SOCKET socket) {
	freeaddrinfo(address_info);
	address_info = nullptr;
	closesocket(socket);
	WSACleanup();
}

WSAData Socket::wsa_data;