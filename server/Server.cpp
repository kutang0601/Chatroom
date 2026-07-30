#include "Server.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

Server::Server(const std::string& ip, int port)
{
    port_ = port;
    listenfd_ = 0;
    memset(&server_addr_, 0, sizeof(server_addr_));
    ip_ = ip;
}

void Server::Start(Server server)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd == -1)
    {
        throw std::runtime_error("socket create fail!");
    }

    server.listenfd_ = listenfd;

    server.server_addr_.sin_family = AF_INET;
    server.server_addr_.sin_port = htons(server.port_);
    server.server_addr_.sin_addr.s_addr = inet_pton(AF_INET, server.ip_.c_str(), &server_addr_.sin_addr);

    if(bind(server.listenfd_, (struct sockaddr*)& server.server_addr_, sizeof(server.server_addr_)) == -1)
    {
        throw std::runtime_error("bind fail!");
    }

    listen(server.listenfd_, 5);

}