/* Made by Liran Dagan 215609397 */
#include "socket_setups.h"
#include "interact_with_server.h"
using namespace std;

/* In main we do all socket procedures from socket_setups.h.
   We check for errors in each procedure. 
   We then create 2 threads, one for each function: sendMessage, receiveMessage
   and we give both of them the socket with which we communicate with the server. 
   By joining the threads we communicate with the server while having the two functions enable
   each other. receiveMessage reacts to the server's feedback to sendMessage, and
   sendMessage adjusts it's actions by reacting to receiveMessage's reponse to the server's
   feedback. As a result we get one coherent application that sends and receives messages
   at the same time. */
int main();

