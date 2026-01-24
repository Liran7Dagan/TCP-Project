/* Made by Liran Dagan 215609397 */
#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

/*
These are the steps the server handles to perform socket interactions:
1. Initialize winsock lib
2. Create the server socket
3. Bind to 0.0.0.0 and port (which is 12345) to the socket
-------------------------
4. Listen on the socket
5. Accept clients
-------------------------
6. 'recv' for receiving messages from clients and 'send' for forwarding them to other clients
-------------------------
We would close the socket and clean up too, but the server never stops running anyway so it's not crucial to mention

The goal: Use 'recv' to get inputs from the users, Use 'send' to instruct users and
forward their messages onwards
*/
class SocketSetups {
public:
	// step 1: Initialize WinSock version 2.2
	static bool initialize();

	// step 2: Create a listening TCP socket for the server to accpet clients
	static SOCKET createTCPsocket();

	// step 3.1: Create the server's Address details, ip and port 
	static bool createAddress(sockaddr_in& serverAddr, const string& ip, const int portNumber);

	// step 3.2: Bind our TCP listening socket to the address details we created
	static bool assignAddress(const SOCKET& listenSocket, sockaddr_in& serverAddr);
};



