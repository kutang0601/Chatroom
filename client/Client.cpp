#include "Client.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Client::Client(const std::string& ip, int port)
{
    socketfd_ = -1;
    port_ = port;
    ip_ = ip;
    running_ = false;
    memset(&server_addr_, 0, sizeof(server_addr_));
}

Client::~Client()
{
    running_ = false;
    CloseConnection();
}

bool Client::GetRunning()
{
    return running_.load();
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

    int connect_ret = connect(socketfd_, (struct sockaddr *)&server_addr_, sizeof(server_addr_));

    if (connect_ret == -1)
    {
        CloseConnection();
        throw std::runtime_error("connect fail!!!");
    }

    std::cout << "connect success" << std::endl;

    running_ = true;
}

bool Client::Send(const std::string& message)
{
    if (!running_.load() || socketfd_ == -1)
    {
        return false;
    }

    ssize_t send_len = send(socketfd_, message.c_str(), message.size(), 0);

    if (send_len == -1)
    {
        std::cout << "send fail!" << std::endl;
        running_ = false;
        CloseConnection();
        return false;
    }

    return true;
}

void Client::Recv()
{
    while (running_.load())
    {
        char buf[1024];

        memset(buf, 0, sizeof(buf));

        int recv_ret = recv(socketfd_, buf, sizeof(buf), 0);

        if (recv_ret > 0)
        {
            std::string message(buf);

            if (message == "server close!")
            {
                std::cout << "服务器关闭！" << std::endl;
                running_ = false;
                CloseConnection();
                break;
            }

            std::cout << buf << std::endl;
        }
        else if (recv_ret == 0)
        {
            std::cout << "服务器终止!" << std::endl;
            running_ = false;
            CloseConnection();
            break;
        }
        else
        {
            std::cout << "recv fail!" << std::endl;
            running_ = false;
            CloseConnection();
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

void *Client::RecvEntrance(void *arg)
{
    Client *client = (Client *)arg;

    client->Recv();

    return nullptr;
}

void Client::CloseConnection()
{
    if (socketfd_ != -1)
    {
        int fd = socketfd_;
        socketfd_ = -1;
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
}