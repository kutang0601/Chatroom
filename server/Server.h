#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

struct ThreadParameter
{
    int clientfd;
};

class Server
{
    public:
        Server(const std::string& ip, int port);

        static void* Entrance(void* arg);

        void Start();

        void Accept();

    private:
        int listenfd_;
        int port_;
        sockaddr_in server_addr_;
        std::string ip_;
};

#endif