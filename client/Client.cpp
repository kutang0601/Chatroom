#include "Client.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Client::Client(const std::string& ip, int port)
{
    socketfd_ = -1;
    port_ = port;
    ip_ = ip;
    memset(&server_addr_, 0, sizeof(server_addr_));
}
Client::~Client()
{
    if (socketfd_ != -1)
    {
        close(socketfd_);
    }
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

    if (pton_ret <= 0)
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

void Client::Recv()
{
    while (1) 
    {
        char buf[1024];

        memset(buf, 0, sizeof(buf));

        int recv_ret = recv(socketfd_, buf, sizeof(buf), 0);
        
        if (recv_ret > 0)
        {
            std::cout << buf << std::endl;
        }
        else if (recv_ret == 0) 
        {
            std::cout << "服务器终止!" << std::endl;
            break;
        }
        else 
        {
            std::cout << "recv fail!" << std::endl;
            break;
        }
    }
}

void Client::RecvThreadStart()
{
    int pthread_ret = pthread_create(&tid_, nullptr, RecvEntrance, this);   

    if (pthread_ret != 0)
    {
        throw std::runtime_error("pthread create fail");
    }

    pthread_detach(tid_);
}

void* Client::RecvEntrance(void* arg)
{
    Client* client = (Client*)arg;

    client->Recv();

    return nullptr;
}