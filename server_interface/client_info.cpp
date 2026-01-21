/* Made by Liran Dagan 215609397 */
#include "client_info.h"

/* FULL CLASS DESCRIPTION IN client_info.h */

// Compare two clients
bool ClientInfo::operator==(const ClientInfo& b) 
{
	return this->name == b.name && this->socket == b.socket;
}

// Empty constructor, name is empty and socket is invalid
ClientInfo::ClientInfo() {
	this->name = {};
	this->socket = INVALID_SOCKET;
}

// Constructor given both client's name and socket
ClientInfo::ClientInfo(string& name, SOCKET socket) 
{
	this->name = name;
	this->socket = socket;
}

// Find a client that goes by 'name' in a 'clients' list
ClientInfo* ClientInfo::findByName(const string& name, list<ClientInfo>& clients)
{
	for (auto& c : clients)
	{
		if (c.name == name)
			return &c;
	}
	return NULL;
}

// Send a given message to the client's socket
bool ClientInfo::getMessage(const string& message)
{
	return send(this->socket, message.c_str(), message.length(), 0) != SOCKET_ERROR;
}

// Delete this client from a list of 'all_clients'
void ClientInfo::deleteFromList(list<ClientInfo>& all_clients)
{
	list<ClientInfo>::iterator removeLocation = find(all_clients.begin(), all_clients.end(), *this);
	if (removeLocation != all_clients.end()) 
	{
		closesocket(this->socket);
		all_clients.erase(removeLocation);
	}
}