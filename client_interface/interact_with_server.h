/* Made by Liran Dagan 215609397 */
#pragma once
#include <WinSock2.h> 
#include <iostream>
#include <thread>
#include <string>

using namespace std;

enum Session {
    REGISTER,
    SEARCH_TARGET,
    CHAT
};
/* Given 2 strings, compare them case isensitively(for example "liRaN" == "LIRan") */
bool equalStrings(const string&, const string&);

// Send client's name to the server to be known. Return success (true) or fail (false)
bool registerClient(SOCKET& server);


/* Send name of another client we want to message. Server will respond with a feedback message.
   Return success (true) or fail (false) */
bool reachTarget(SOCKET& server);


/* A thread function. Here we handle all of client's messages to the server in correlation to 'mode'
  'sendMessage' utillizes all the functions above. 
  In short we do the following:
  1. Use the 'mode' variable to distinguish between actions as followed:
     mode = REGISTER -> Send client's name to server (registerClient)
     mode = SEARCH_TARGET -> Specify target (searchTarget)
     mode = CHAT -> Send message to server to deliver to target
  2. If connected==true meaning client didn't quit and no connection error occured then go to 1.
     else quit function */
void sendMessage(SOCKET server);


/* A thread function. Here we handle all of server's messages to the client including error feedbacks
   and messages from other clients.
   In short we do the following:
   0. Get message/prompt from server 
   1. if got sameUser or UserNotFound prompts then stay in mode = 1 for sendMessage
   2. if got userFound prompt then procced to mode = 2 for sendMessage
   3. if got Quit prompt or a disconnection error then connected=false and 6.
   4. if none of the above then we actually got a message from another client and not a prompt, so we display it
   5. Go to 0. 
   6. quit both thread functions sendMessage, receiveMessage and return to main to finish program */
void receiveMessage(SOCKET server);


