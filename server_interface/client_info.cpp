/* Made by Liran Dagan 215609397 */
#include "client_info.h"

bool ClientInfo::operator==(const ClientInfo& b) {
	return this->name == b.name && this->socket == b.socket;
}

ClientInfo::ClientInfo() {
	this->name = {};
	this->socket = INVALID_SOCKET;
}

ClientInfo::ClientInfo(string& name, SOCKET socket) {
	this->name = name;
	this->socket = socket;
}

ClientInfo* ClientInfo::findByName(const string& name, list<ClientInfo>& clients) {
	for (auto& c : clients)
	{
		if (c.name == name)
			return &c;
	}
	return NULL;
}
bool ClientInfo::getMessage(const string& message)
{
	return send(this->socket, message.c_str(), message.length(), 0) != SOCKET_ERROR;
}
void ClientInfo::deleteFromList(list<ClientInfo>& all_clients)
{
	list<ClientInfo>::iterator removeLocation = find(all_clients.begin(), all_clients.end(), *this);
	if (removeLocation != all_clients.end()) {
		closesocket(this->socket);
		all_clients.erase(removeLocation);
	}
}