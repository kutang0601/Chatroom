#include "ClientManager.h"
#include "ClientConnection.h"

#include <string>

ClientManager::ClientManager()
{

}

ClientManager::~ClientManager()
{
    for (auto n = clients_.begin(); n != clients_.end(); n++) 
    {
        delete n->second;
    }

    clients_.clear();
}

bool ClientManager::Add(ClientConnection* client)
{
    int id = client->GetId();

    if (clients_.find(id) != clients_.end())
        return false;

    clients_[id] = client;

    return true;
}

bool ClientManager::Remove(ClientConnection* client)
{
    if (clients_.find(client->GetId()) == clients_.end())
        return false;

    if (clients_[client->GetId()] != client)
        return false;

    clients_.erase(client->GetId());
    delete client;

    return true;
}

void ClientManager::Broadcast(ClientConnection* sender, const std::string& message)
{
    for (auto n = clients_.begin(); n != clients_.end() ; n++)
    {
        if (n->second != sender)
        {
            n->second->Send(message);
        }
    }
}