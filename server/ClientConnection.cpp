#include "ClientConnection.h"

#include <bits/types/cookie_io_functions_t.h>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

ClientConnection::ClientConnection(int fd)
{
    fd_ = fd;
}

ClientConnection::~ClientConnection()
{
    close(fd_);
}

std::string ClientConnection::Recv()
{
    char message[1024];
    memset(message, 0, sizeof(message)); 

    int recv_ret = recv(fd_, message, sizeof(message), 0);

    std::string ret; 

    if (recv_ret > 0)
    {
        std::cout << message << std::endl;
        
        return message;
    }
    else if (recv_ret == 0)
    {
        ret = "客户端退出";
        return ret;
    }
    else
    {
        ret = "recv fail";
        return ret;
    }
}

void ClientConnection::Send(const std::string& message)
{
    ssize_t send_len = send(fd_, message.c_str(), message.size(), 0);

    if (send_len == -1)
    {
        throw std::runtime_error("send fail!");
    }
}