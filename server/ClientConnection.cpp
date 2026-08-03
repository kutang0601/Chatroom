#include "ClientConnection.h"

#include <bits/types/cookie_io_functions_t.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>

ClientConnection::ClientConnection(int fd)
{
    fd_ = fd;
}

ClientConnection::~ClientConnection()
{
    
}

void ClientConnection::Recv()
{
    while(1)
    {
        char message[1024];
        memset(message, 0, sizeof(message)); 

        int recv_ret = recv(fd_, message, sizeof(message), 0);

        if (recv_ret > 0)
        {
            std::cout << message << std::endl;
        }
        else if (recv_ret == 0)
        {
            std::cout << "客户端退出" << std::endl;
            break;
        }
        else
        {
            std::cout << "recv fail!" << std::endl;
            break;
        }
    }
}
