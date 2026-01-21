/* Made by Liran Dagan 215609397 */
#include "interact_with_server.h"

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

bool registerClient(SOCKET& server)
{
	string name;
	cout << "Welcome to the chat! What is your name?" << endl;
	do {
		getline(cin, name);
		if (equalStrings(name, "QUIT")) // If got an early quit then don't bother registering
			return false;
	} while (name.empty());
	cout << "Hello " + name + "! Have fun chatting." << endl << "You can quit any time by typing 'QUIT'." << endl << "-----------------------------------------" << endl;
	if (send(server, name.c_str(), name.length(), 0) == SOCKET_ERROR) {
		cout << "Error sending name" << endl;
		return false;
	}
	return true;
}

bool reachTarget(SOCKET& server)
{
	string targetName;
	cout << "Who do you want to message?" << endl;
	do {
		getline(cin, targetName);
	} while (targetName.empty());
	if (send(server, targetName.c_str(), targetName.length(), 0) == SOCKET_ERROR)
	{
		cout << "Error sending name request" << endl;
		return false;
	}
	Sleep(500);
	return true;
}

/* 'mode' gets the values:
REGISTER: Enter client's name to be stored at server
SEARCH_TARGET: Search for a specified receiver side by his name
CHAT: Deliver messages to the receiver (By delivering them to the server to handle): (Client (we're here) -> Server -> Receiver)
*/
Session mode;
/* Connection to server. When false (by typing 'quit'), the program ends
(if connected == false then threads sendMessage, receiveMessage finish their run) */
bool connected = true;
/* in mode 1 we send a target name to the server who then replies with feedback(success / error) : This flag is the stores the answer */
bool foundTarget = false;

void sendMessage(SOCKET server)
{
	string input;
	while (connected)
	{
		switch (mode)
		{
		case 0:
			if (registerClient(server)) // Send name to server, it will handle the registering and recognize us from now on
				mode = SEARCH_TARGET; // Move to the next step, specify target to message
			else
				connected = false;
			break;
		case 1: // We contact a desired target by name and wait for an approval from the server (see function 'receiveMessage' shortly)
			if (!foundTarget)
			{
				if (!reachTarget(server)) // If we haven't acquired a target yet, send the request to the server
					connected = false; //  If we failed to send the request (due to entering "quit" or disconnection error), stop running
			}
			break;
		case 2: // Deliver messages to server, it then will handle the forwarding to our desired target
			do {
				getline(cin, input);
			} while (input.empty());
			cout << endl;
			if (equalStrings(input, "BACK")) // Enter 'Back' to go back to mode 1 if we wish to swap between desired targets. Without this - We are limited to access one client only
			{
				mode = SEARCH_TARGET;
				foundTarget = false;
			}
			if (equalStrings(input, "QUIT")) // Enter 'Quit' to stop messaging and quit the chat
				connected = false;
			if (send(server, input.c_str(), input.length(), 0) == SOCKET_ERROR)
				cout << "Error sending message" << endl;
			Sleep(500);
			break;
		}
	}
}

void receiveMessage(SOCKET server)
{
	char buffer[4096];
	int bytesRecv;
	string message; // Server's message/feedback to the client
	while (connected) {
		if (mode == REGISTER) continue; // If haven't registered yet then wait until then
		bytesRecv = recv(server, buffer, sizeof(buffer), 0);
		if (bytesRecv <= 0) // An unexpected disconnection / server error 
		{
			cout << "Disconnected from server" << endl;
			connected = false;
			break;
		}
		message = string(buffer, bytesRecv); // Load the server's message to the client
		if (message == "S:SameUser") // If the client chose himself as a target, We return to mode 1 in 'SendMessage'
		{
			cout << "Please Message a Different User." << endl << "-----------------------------------------" << endl;
			foundTarget = false;
			mode = SEARCH_TARGET; // Go back to search a different target
		}
		else if (message == "S:UserNotFound") // If the user chose a non existing target (a name that is not in the chat), We return to mode 1 in 'SendMessage'
		{
			cout << "User Was Not Found." << endl << "-----------------------------------------" << endl;
			foundTarget = false;
			mode = SEARCH_TARGET; // Go back to search a different target
		}
		else if (message == "S:UserFound") // If we successfully found our target, we procced to mode 2 in 'SendMessage'
		{
			cout << "Connection created. You can message a different user by typing 'BACK'" << endl << "------------------------------------------------------------------------" << endl;
			foundTarget = true;
			mode = CHAT; // Procceed to send Messages to the target
		}
		else if (message == "S:TargetDC") // If the target quit mid conversation, return to mode 1
		{
			cout << "The other party has disconnected." << endl << "-----------------------------------------" << endl;
			foundTarget = false;
			mode = SEARCH_TARGET; // Go back to search a different target
		}
		else if (message == "S:quit") // If we sent quit then stop looping
			connected = false;
		// else this is not a server reply but an incoming message given from a different client! In fact 'sendMessage' was applied on us by a different host
		// and we are the receiver "target" side (client (we're here) <- server <- other client)
		// The server tags other clients' messages with a prefix "C:", so we print from index 2
		else {
			cout << message.c_str() + 2 << endl << endl;
			if (mode == SEARCH_TARGET) // In case we got a message from someone else before acquiring a target ourselves, we print the message but also remind the user to select a target (For clarity)
				cout << "Who do you want to message?" << endl;
		}
	}
}