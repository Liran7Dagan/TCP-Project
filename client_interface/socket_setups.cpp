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
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return clientSocket;
}

// step 3.1
bool SocketSetups::createAddress(sockaddr_in& serverAddr, const string& ip, const int portNumber)
{
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNumber);
	return inet_pton(AF_INET, ip.c_str(), &(serverAddr.sin_addr)) == 1;
}

// step 3.2
bool SocketSetups::connectToServer(SOCKET& clientSocket, sockaddr_in& serverAddr)
{
	return connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) != SOCKET_ERROR;
}