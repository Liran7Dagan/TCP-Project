/* Made by Liran Dagan 215609397 */
#include "main.h"
#include "interact_with_client.h"

using namespace std;

int main()
{
	// step 1: Start winsock
	if (!SocketSetups::initialize()) {
		cout << "Failed to initialize the server." << WSAGetLastError() << endl;
		return -1;
	}

	// step 2: create TCP listening socket
	SOCKET listenSocket = SocketSetups::createTCPsocket();
	if (listenSocket == INVALID_SOCKET) {
		cout << "Failed to create server socket" << WSAGetLastError() << endl;
		return -1;
	}

	// step 3.1: Server Address struct - here we set the server's address and port number
	sockaddr_in serverAddr;
	const string ip = "0.0.0.0"; // Listen to all local IPs
    const int port = 12345; 
	if (!SocketSetups::createAddress(serverAddr, ip, port))
	{
		cout << "Setting address failed " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	// step 3.2: Bind the TCP type socket from earlier to the address struct (0.0.0.0,  12345) 
	if (!SocketSetups::assignAddress(listenSocket, serverAddr))
	{
		cout << "Bind failed " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}

	// step 4: The socket is ready, now we're ready to detect clients
	if (listen(listenSocket, MAX_USERS) == SOCKET_ERROR)
	{
		cout << "Listen failed " << WSAGetLastError() << endl;
		closesocket(listenSocket);
		WSACleanup();
		return -1;
	}
	cout << "Server is now listening on port " << port << endl;
	
	list<ClientInfo> clients; // Stores the names & sockets of all clients participating in chat
	while (true) {
		// step 5: Accept and start communicating with clients
		SOCKET clientSocket = accept(listenSocket, NULL, NULL); // A client tries to access the server
		if (clientSocket == INVALID_SOCKET)
		{
			cout << "Client socket is invalid" << endl;
		}
		else  // We split the server's attention to each reaching client, so we get a multi client system
		{
			thread manageClient(Interact, clientSocket, ref(clients)); // Each client gets his own treatment simultaneously with threads
			manageClient.detach();
		}
	}
	closesocket(listenSocket);
	WSACleanup();
	return 0;
}