#include "Client.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>

Client::Client(const std::string& ip, int port)
{
    socketfd_ = -1;
    port_ = port;
    ip_ = ip;
    memset(&server_addr_, 0, sizeof(server_addr_));
}

void Client::Connect()
{
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socketfd == -1)
    {
        throw std::runtime_error("socket create fail!");
    }

    socketfd_ = socketfd;

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    int pton_ret = inet_pton(AF_INET, ip_.c_str(), &server_addr.sin_addr);

    if (pton_ret == -1)
    {
        throw std::runtime_error("pton fail!");
    }

    server_addr_ = server_addr;

    int connect_ret = connect(socketfd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));

    if (connect_ret == -1)
    {
        throw std::runtime_error("connect fail!!!");
    }

    std::cout << "connect success" << std::endl;
}

void Client::Send(const std::string& message)
{
    ssize_t send_len = send(socketfd_, message.c_str(), message.size(), 0);

    if (send_len == -1)
    {
        throw std::runtime_error("send fail!");
    }
}