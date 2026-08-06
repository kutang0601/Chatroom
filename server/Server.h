#ifndef SERVER_H
#define SERVER_H

#include "ClientConnection.h"
#include "ClientManager.h"

#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

struct ThreadParameter
{
    ClientManager* manager;
    std::shared_ptr<ClientConnection> client;
};

class Server
{
    public:
        Server(const std::string& ip, int port);

        static void* ServerEntrance(void* arg);

        void Start();

        void Accept();

    private:
        int listenfd_;
        int port_;
        int next_id_ = 1;
        sockaddr_in server_addr_;
        std::string ip_;
        ClientManager manager_;
};

#endif