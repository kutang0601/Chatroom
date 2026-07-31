#include "Server.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

Server::Server(const std::string &ip, int port) {
  port_ = port;
  listenfd_ = 0;
  memset(&server_addr_, 0, sizeof(server_addr_));
  ip_ = ip;
}

void Server::Start()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd == -1)
    {
        throw std::runtime_error("socket create fail!");
    }

    listenfd_ = listenfd;

    memset(&server_addr_, 0, sizeof(server_addr_));

    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    
    int pton_ret = inet_pton(AF_INET, ip_.c_str(), &server_addr_.sin_addr);

    if (pton_ret <= 0)
    {
        throw std::runtime_error("pton fail");
    }

    if(bind(listenfd_, (sockaddr*)& server_addr_, sizeof(server_addr_)) == -1)
    {
        throw std::runtime_error("bind fail!");
    }

    int listen_ret = listen(listenfd_, 5);

    if (listen_ret == -1)
    {
        throw std::runtime_error("listen fail!");
    }

    Accept();
    
}

void Server::Accept()
{
    sockaddr_in client_addr;

    socklen_t len = sizeof(client_addr);

    int clientfd = accept(listenfd_, (sockaddr*) &client_addr, &len);

    if (clientfd == -1)
    {
        throw std::runtime_error("accept fail");
    }

    clientfd_ = clientfd;

    std::cout << "client accept success!" << std::endl;
}

void Server::Recv()
{
    char buf[1024];

    memset(buf, 0, sizeof(buf));

    if ((recv(clientfd_, buf, sizeof(buf), 0) == -1))
    {
        throw std::runtime_error("recv fail");
    }

    std::cout << buf << std::endl;
}