/* Made by Liran Dagan 215609397 */
#include "socket_setups.h"

// step 1
bool SocketSetups::initialize()
{
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

// step 2
SOCKET SocketSetups::createTCPsocket()
{
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return listener;
}

// step 3.1
bool SocketSetups::createAddress(sockaddr_in& serverAddr, const string& ip, const int portNumber)
{
	serverAddr.sin_family = AF_INET; // Server connection family, TCP or UDP
	serverAddr.sin_port = htons(portNumber);// htons ("host to network short") converts the port number to network byte order.

	// inet_pton converts 'ip' (which is 0.0.0.0) to binary.
	// Later when using bind() the server will bind it's socket to listen to all local interfaces.
	return inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) == 1;
}

// step 3.2
bool SocketSetups::assignAddress(const SOCKET& listenSocket, sockaddr_in& serverAddr)
{
	return bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) != SOCKET_ERROR;
}