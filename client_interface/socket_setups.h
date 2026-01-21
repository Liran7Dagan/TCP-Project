/* Made by Liran Dagan 215609397 */
#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

/*
These are the steps the client handles to perform socket interactions:
1. Initialize winsock
2. Create comuunicating socket
3. Connect to server
4. 'send' for delivering messages to server and 'recv' for receiving messages from server
5. Close the socket when finished
6. Cleanup

The goal: Use 'send' and 'recv' methods with the server in order to
          1. forward messages to a specified client
          2. get messages from other clients

*/
class SocketSetups
{
public:
    // step 1: Initialize WinSock version 2.2
    static bool initialize();

    // step 2: Create a TCP socket to contact the server with
    static SOCKET createTCPsocket();

    // step 3.1: Create the address details of the server the client connects - ip and port
    static bool createAddress(sockaddr_in& serverAddr, const string& ip, const int portNumber);

    // step 3.2: Connect to the server using the 'connect' function
    static bool connectToServer(SOCKET& serverSocket, sockaddr_in& serverAddr);
};
