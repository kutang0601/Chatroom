#include "Client.h"

#include <iostream>
#include <string>

int main()
{
    try 
    {
        Client client("127.0.0.1", 8080);

        client.Connect();

        client.RecvThreadStart();

        while (1)
        {
            std::string message;
            
            std::getline(std::cin, message);

            if (!message.compare("exit"))
                break;

            client.Send(message);
        }

    } 
    catch (const std::exception& e) 
    {
        std::cout << e.what() << std::endl;
    }


    return 0;
}