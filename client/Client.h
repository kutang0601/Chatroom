#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>
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

        bool GetRunning();

        bool Send(const std::string& message);

        void Recv();

    private:
        void CloseConnection();

        int socketfd_;
        int port_;
        std::atomic<bool> running_;
        pthread_t tid_;
        std::string ip_;
        struct sockaddr_in server_addr_;
};

#endif