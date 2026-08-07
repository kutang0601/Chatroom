#include "ClientManager.h"
#include "ClientConnection.h"

#include <iostream>
#include <memory>
#include <pthread.h>
#include <string>
#include <vector>

ClientManager::ClientManager()
{
    int init_ret = pthread_mutex_init(&mutex_, nullptr);

    if (init_ret != 0)
    {
        std::cout << "pthread mutex fail!" << std::endl;
    }
}

ClientManager::~ClientManager()
{
    pthread_mutex_lock(&mutex_);

    for (auto it : clients_)
    {
        it.second->Close();
    }

    clients_.clear();

    pthread_mutex_unlock(&mutex_);

    pthread_mutex_destroy(&mutex_);
}

bool ClientManager::Add(std::shared_ptr<ClientConnection> client)
{
    int id = client->GetId();

    pthread_mutex_lock(&mutex_);

    if (clients_.find(id) != clients_.end())
    {
        pthread_mutex_unlock(&mutex_);

        return false;
    }
    clients_[id] = client;

    pthread_mutex_unlock(&mutex_);

    return true;
}

bool ClientManager::Remove(int id)
{
    std::shared_ptr<ClientConnection> client;

    pthread_mutex_lock(&mutex_);

    auto it = clients_.find(id);

    if (it == clients_.end())
    {
        pthread_mutex_unlock(&mutex_);

        return false;
    }

    client = it->second;

    clients_.erase(it);

    pthread_mutex_unlock(&mutex_);

    client->Close();

    return true;
}

void ClientManager::Broadcast(std::shared_ptr<ClientConnection> sender, const std::string& message)
{
    std::vector<std::shared_ptr<ClientConnection>> temp;

    pthread_mutex_lock(&mutex_);

    for (auto n = clients_.begin(); n != clients_.end(); n++)
    {
        temp.push_back(n->second);
    }

    pthread_mutex_unlock(&mutex_);

    for (auto it : temp)
    {
        if (it != sender && !it->Send(message))
        {
            Remove(it->GetId());
        }
    }
}

void ClientManager::NotifyAll(const std::string& message)
{
    std::vector<std::shared_ptr<ClientConnection>> temp;

    pthread_mutex_lock(&mutex_);

    for (auto n = clients_.begin(); n != clients_.end(); n++)
    {
        temp.push_back(n->second);
    }

    pthread_mutex_unlock(&mutex_);

    for (auto it : temp)
    {
        if (!it->Send(message))
        {
            Remove(it->GetId());
        }
    }
}

