#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class Server
{
    public:
        Server(const std::string& ip, int port);

        void Start();

        void Accept();

        void Recv();

    private:
        int listenfd_;
        int clientfd_;
        int port_;
        sockaddr_in server_addr_;
        std::string ip_;
};

#endif