#include "Server.h"
#include "ClientConnection.h"
#include "ClientManager.h"
#include "RecvResult.h"

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <netinet/in.h>
#include <pthread.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(const std::string &ip, int port) 
{
    port_ = port;
    listenfd_ = -1;
    memset(&server_addr_, 0, sizeof(server_addr_));
    ip_ = ip;
    running_ = true;
}

void Server::Start()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd == -1)
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

    if (bind(listenfd_, (sockaddr*)& server_addr_, sizeof(server_addr_)) == -1)
    {
        throw std::runtime_error("bind fail!");
    }

    int listen_ret = listen(listenfd_, 5);

    if (listen_ret == -1)
    {
        throw std::runtime_error("listen fail!");
    }

    while (running_)
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
        if (!running_)
            return;

        throw std::runtime_error("accept fail");
    }

    int id = next_id_++;

    std::shared_ptr<ClientConnection> client =  std::make_shared<ClientConnection>(clientfd, id);

    if (!manager_.Add(client))
    {
        client->Close();

        throw std::runtime_error("clients key reapeat!");
    }

    ThreadParameter* parameter = new ThreadParameter;
    
    parameter->client = client;
    parameter->manager = &manager_;

    pthread_t tid = 0;

    int thread_ret = pthread_create(&tid, nullptr, ServerEntrance, parameter);

    if (thread_ret != 0)
    {   
        manager_.Remove(client->GetId());

        throw std::runtime_error("pthread create fail");
    }

    pthread_detach(tid);
}

void* Server::ServerEntrance(void* arg)
{
    ThreadParameter* paremeter = (ThreadParameter*)arg;

    std::shared_ptr<ClientConnection> client = paremeter->client;
    ClientManager* manager = paremeter->manager;

    delete paremeter;

    while (1)
    {
        struct RecvResult* result = client->Recv();

        switch (result->status) 
        {
            case RecvStatus::RECV_SUCCESS:
            {    
                manager->Broadcast(client, result->message);

                delete result;
            }
                break;
            
            case RecvStatus::RECV_ERROR:
            {
                manager->Remove(client->GetId());

                delete result;

                return nullptr;
            }
            break;

            case RecvStatus::CLIENT_EXIT:
            {
                manager->Remove(client->GetId());

                delete result;

                return nullptr;
            }
        }
    }
    
    return nullptr;
}

void Server::Stop()
{
    running_ = false;

    if (listenfd_ != -1)
    {
        shutdown(listenfd_, SHUT_RDWR);

        close(listenfd_);
        listenfd_ = -1;
    }

    manager_.NotifyAll("server close!");
}