#include "Server.h"

#include <exception>
#include <iostream>

int main()
{
   try 
   {
        Server server("127.0.0.1", 8080);

        server.Start();

   } 
   catch (const std::exception& e) 
   {
    std::cout << e.what() << std::endl;
   }

    return 0;
}