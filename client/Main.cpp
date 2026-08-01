#include "Client.h"
#include <iostream>
#include <string>

int main()
{
    try 
    {
        Client client("127.0.0.1", 8080);

        client.Connect();

        std::string message = "hello world";

        client.Send(message);

    } 
    catch (const std::exception& e) 
    {
        std::cout << e.what() << std::endl;
    }


    return 0;
}