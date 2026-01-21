/* Made by Liran Dagan 215609397 */
#pragma once
#include <iostream>
#include <string>
#include <list>
#include "client_info.h"
using namespace std;

enum Session {
    REGISTER,
    SEARCH_TARGET,
    DELIVER_TO_TARGET
};
/* Given 2 strings, compare them case isensitively(for example "liRaN" == "LIRan") */
bool equalStrings(const string& str1, const string& str2);

/* Given any client's message to the server we first have to check if he entered 'quit' 
   If he did, then we notify the client on his disconnection and return true
   If he didn't, we return false */
bool isQuit(string& message, ClientInfo& client);

/* Given a client's input of a target he wants to message, the server checks if the target
   is valid and sends feedback to the client. Errors to be detected are:
   1. target is the client himself (logical error, client is trying to message himself).
   2. target doesn't exist (technical error, the target can not be found) 
   If there is no error we notify the client that we found the target, else we
   send the error to the client. */
bool validateTarget(string& targetName, ClientInfo& client, list<ClientInfo>& all_clients);

/* Once the target is validated, we need to make sure they're still active on send. 
   If the target has disconnected mid conversation we notify the client that the connection is closed
   and ask him to pick a different target */
ClientInfo* getTarget(string& targetName, ClientInfo& client, list<ClientInfo>& all_clients);

/* 'Interact' is the main function that manages the interactions between the server and the client.
   It Handles the server's actions with a single client and forwards messages from the client
   to a target client.
   If needed, the server sends feedback messages regarding errors.
   Interact utiliizes all the functions from above.
   
   In short, we do the following:
   0. We got a message from the client
   1. Check for 'quit' (isQuit), if the client didn't send quit we procced to 2., else we stop running 
   and remove him (deleteClient).
   2. Use a 'mode' variable that gets 0, 1 or 2 to distinguish between actions as followed:
      mode = REGISTER -> We receive client's name and save his details (createClient)
      mode = SEARCH_TARGET -> we receive the target client and check for errors (isError)
      mode = DELIVER_TO_TARGET -> We got a message to deliver and we send it to the target client by finding his socket in the clients list
   3. return to 0.
   */
  
void Interact(SOCKET clientSocket, list<ClientInfo>& all_clients);


