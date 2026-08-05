#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <string>

class ClientConnection
{
    public:
        ClientConnection(int fd, int id);

        ~ClientConnection();

        int GetId();

        std::string Recv();

        void Send(const std::string& message);

    private:
        int fd_;
        int id_;
};

#endif