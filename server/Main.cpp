#include "Server.h"

#include <csignal>
#include <exception>
#include <iostream>
#include <signal.h>

Server* server_tmp = nullptr;

void SignalHandler(int signal_send)
{
     if (signal_send == SIGINT)
     {
          if (server_tmp)
          {
               server_tmp->Stop();
          }
     }
}

int main()
{
     try 
     {
          Server server("127.0.0.1", 8080);
          
          server_tmp = &server;

          signal(SIGINT, SignalHandler);

          server.Start();

     } 
     catch (const std::exception& e) 
     {
     std::cout << e.what() << std::endl;
     }

    return 0;
}