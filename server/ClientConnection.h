#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <string>

class ClientConnection
{
    public:
        ClientConnection(int fd);

        ~ClientConnection();

        std::string Recv();

        void Send(const std::string& message);

    private:
        int fd_;
};

#endif