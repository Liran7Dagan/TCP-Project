/* Made by Liran Dagan 215609397 */
#pragma once
#include <iostream>
#include <tchar.h>
#include <thread>
#include <list>
#include "client_info.h"
#include "socket_setups.h"
#define MAX_USERS 100
using namespace std;

/* In main we do all socket procedures from socket_setups.h along with listening and 
   accepting clients. We check for errors in each procedure. We then create an empty
   list of type ClientInfo in which we will store all the active clients. 
   Once a client comes in and we accept him, we send his socket to the 'Interact'
   function from interact_with_client.h by a thread to isolate the server and him, along with a
   reference to the list of clients we created.
   By using threads we split the server's attention to each client in particular, creating desynchronization
   which is essential for chat systems */
int main();