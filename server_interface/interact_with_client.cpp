/* Made by Liran Dagan 215609397 */
#include "interact_with_client.h"

using namespace std;

/* Given 2 strings, compare them case isensitively (for example "liRaN" == "LIRan") */
bool equalStrings(const string& str1, const string& str2)
{
	int i;
	for (i = 0; str1[i] && str2[i]; i++)
	{
		if (toupper(str1[i]) != toupper(str2[i]))
			return false;
	}
	return !str1[i] && !str2[i];
}
bool isQuit(string& message, ClientInfo& client)
{
	const static string quitProgram = "S:quit";
	if (equalStrings(message, "QUIT"))
	{
		if (!client.getMessage(quitProgram)) {
			cout << "Failed to disconnect '" << client.name << "' from server" << endl;
			return false;
		}
		cout << "'" << client.name << "' Disconnected" << endl;
		return true;
	}
	return false;
}

bool validateTarget(string& targetName, ClientInfo& client, list<ClientInfo>& all_clients)
{
	/* Side Note : We give 'serverReply' the prefix "S:" and later we give messages from
	   clients the prefix "C:". Why is that? Without these tags, clients could send fake
	   errors to each other pretending to be the server. We don't want that to happen
	   so we tag server replies and client messages with different prefixes. */
	bool foundError;
	string serverReply;
	if (targetName == client.name) // send 'SameUser' reply if the client chose himself
	{
		foundError = true;
		serverReply = "S:SameUser";
		cout << "'" << client.name << "' Tried to message himself" << endl;
	}
	else if (!ClientInfo::findByName(targetName, all_clients)) // send 'UserNotFound' reply if the target is not in the list 
	{
		foundError = true;
		serverReply = "S:UserNotFound";
		cout << "'" << client.name << "' Tried to reach a non existent client: '" << targetName << "'" << endl;
	}
	else // Else we send 'UserFound' reply, we found the target we were looking for in the list
	{
		foundError = false;
		serverReply = "S:UserFound";
		cout << "'" << client.name << "' Reaches '" << targetName << "'. Server is ready to transmit messages!" << endl;
	}
	if (!client.getMessage(serverReply)) {
		cout << "Failed sending feedback to '" << client.name << "'" << endl;
		foundError = true;
	}
	return foundError;
}
ClientInfo* getTarget(string& targetName, ClientInfo& client, list<ClientInfo>& all_clients)
{
	ClientInfo* targetClient = ClientInfo::findByName(targetName, all_clients); // Check if target hasn't disconnected
	if (targetClient == NULL) {
		cout << "'" << client.name << "'s connection with '" << targetName << "' has been terminated." << endl;
		const static string DCerror = "S:TargetDC";
		if (!client.getMessage(DCerror))
			cout << "Failed sending feedback to '" << client.name << "'" << endl;
	}
	return targetClient;
}
/* step 6: use 'recv','send' and additional logic to communicate with every client
*  summary:
   'Interact' Handles the server's actions with a single client and forwards it's messages to whoever it wishes
   If needed, the server sends feedback messages regarding errors.
   We save a list of all the active clients including the name and socket of each one */
void Interact(SOCKET clientSocket, list<ClientInfo>& all_clients)
{
	Session mode = REGISTER;
	/* mode gets the following values:
	REGISTER = We don't know client, so we create and add them as a new client to the list
	SEARCH_TARGET = Figure out target that client is trying to reach
	CHAT = Deliver client's message to target
	*/
	ClientInfo client; // The client we handle
	string targetName; // The name (identifier) of the target we forward to
	string message; // Our client's input 
	int bytesRecved;
	char buffer[4096];
	while (true) {
		// recv gets message (string) from client and loads it to buffer
		bytesRecved = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRecved <= 0) // If an unexpected error occurs 
		{
			if(!client.name.empty())
				cout << "'" << client.name << "' DISCONNECTED" << endl;
			break;
		}
		message = string(buffer, bytesRecved);
		if (isQuit(message, client)) // If client sent 'Quit' then quit the loop
			break;

		switch (mode) {
		case REGISTER:
			// create the client {name , socket}, Add it to the list and procced to mode 1
			client = { message, clientSocket };
			all_clients.push_front(client);
			cout << "New client joined: '" << message << "'" << endl;
			mode = SEARCH_TARGET;
			break;

		case SEARCH_TARGET: // Figure out the target our client tries to reach
			// Send feedback on search error/success and procced to mode 2 (if target was found) 
			if (!validateTarget(message, client, all_clients))
			{
				targetName = message; // If the search was succesful remember target's name
				mode = DELIVER_TO_TARGET;
			}
			break;

		case DELIVER_TO_TARGET: // Message target client by forwarding ('send') client's message (client -> server (we're here) -> target)
			if (equalStrings(message, "BACK"))
			{
				mode = SEARCH_TARGET;
				break;
			}
			ClientInfo* targetClient = getTarget(targetName, client, all_clients); // Check if target hasn't disconnected
			if (targetClient == NULL) { // Target has disconnected. Reach a different one
				mode = SEARCH_TARGET;
				break;
			}
			// 'display' is the message the client receives. The client will figure this is a message from another client 
			// because of the prefix C: , then he will print it excluding the prefix C:
			string display = "C:" + client.name + ": " + message;
			if (!targetClient->getMessage(display))
				cout << "'" << client.name << "' Failed to send message to '" << targetClient->name << "'" << endl;
			else
				cout << "'" << client.name << "' TO '" << targetClient->name << "': " << message << endl;
			break;
		}
	}
	// If client disconnected we close his socket and remove him from the active clients list	
	client.deleteFromList(all_clients);
}