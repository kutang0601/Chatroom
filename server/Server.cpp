#include "Server.h"
#include "ClientConnection.h"
#include "ClientManager.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <pthread.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(const std::string &ip, int port) {
  port_ = port;
  listenfd_ = -1;
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

    while(1)
    {
        Accept();
    }

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

    int id = next_id_++;

    ClientConnection* client = new ClientConnection(clientfd, id);

    if (!manager_.Add(client))
    {
        delete client;

        throw std::runtime_error("clients key reapeat!");
    }

    ThreadParameter* parameter = new ThreadParameter;
    
    parameter->client = client;
    parameter->manager = &manager_;

    pthread_t tid = 0;

    int thread_ret = pthread_create(&tid, nullptr, ServerEntrance, parameter);

    if (thread_ret != 0)
    {   
        manager_.Remove(client);

        throw std::runtime_error("pthread create fail");
    }

    pthread_detach(tid);
}

void* Server::ServerEntrance(void *arg)
{
    ThreadParameter* paremeter = (ThreadParameter*)arg;

    ClientConnection* client = paremeter->client;
    ClientManager* manager = paremeter->manager;

    delete paremeter;

    while (1)
    {
        std::string message_send = client->Recv();

        if (message_send == "客户端退出") 
        {
            std::cout << message_send << std::endl;

            manager->Remove(client);

            return nullptr;
        }
        else if(message_send == "recv fail")
        {
            std::cout << message_send << std::endl;

            manager->Remove(client);

            return nullptr;
        }
            
        manager->Broadcast(client, message_send);
    }
    
    return nullptr;
}