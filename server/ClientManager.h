#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ClientConnection.h"
#include <map>
#include <string>

class ClientManager
{
    public:
        ClientManager();

        ~ClientManager();

        bool Add(ClientConnection* client);

        bool Remove(ClientConnection* client);

        void Broadcast(ClientConnection* sender, const std::string& message);

    private:
        std::map<int, ClientConnection*> clients_;
};

#endif