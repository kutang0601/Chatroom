#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ClientConnection.h"

#include <map>
#include <memory>
#include <pthread.h>
#include <string>

class ClientManager
{
    public:
        ClientManager();

        ~ClientManager();

        bool Add(std::shared_ptr<ClientConnection> client);

        bool Remove(int id);

        void Broadcast(std::shared_ptr<ClientConnection> sender, const std::string& message);

    private:
        std::map<int, std::shared_ptr<ClientConnection>> clients_;
        pthread_mutex_t mutex_;
};

#endif