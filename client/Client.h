#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <pthread.h>
#include <string>


class Client
{
    public:
        Client(const std::string& ip, int port);

        ~Client();
        
        static void* RecvEntrance(void* arg);

        void RecvThreadStart();

        void Connect();

        void Send(const std::string& message);

        void Recv();

    private:
        int socketfd_;
        int port_;
        pthread_t tid_;
        std::string ip_;
        struct sockaddr_in server_addr_;
};

#endif