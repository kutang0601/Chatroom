#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <string>


class Client
{
    public:
        Client(const std::string& ip, int port);

        void Connect();

        void Send(const std::string& message);

    private:
        int socketfd_;
        int port_;
        std::string ip_;
        struct sockaddr_in server_addr_;
};

#endif