/* Made by Liran Dagan 215609397 */
#pragma once
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <list>

using namespace std;

// Each client has a name and a socket to which the server sends info to and receives info from
class ClientInfo {
public:
	string name;
	SOCKET socket;

public:
	ClientInfo();
	ClientInfo(string& name, SOCKET socket);
	bool operator==(const ClientInfo&);

	// A method to find a specific client in a list, We are given that all client names are unique
	// This is used when a client specifies the name of the target they wish to message.
	static ClientInfo* findByName(const string&, list<ClientInfo>&);
	
	// Given a message, deliver it to this client via the socket with "send" function. true for a successful send, false for a failed send
	bool getMessage(const string& message);

	// Given a list containing this client, we remove the client from the list.
	void deleteFromList(list<ClientInfo>& all_clients);
};