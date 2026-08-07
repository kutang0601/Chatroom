#include "ClientConnection.h"
#include "RecvResult.h"

#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

ClientConnection::ClientConnection(int fd, int id)
{
    fd_ = fd;
    id_ = id;
}

ClientConnection::~ClientConnection()
{
    Close();
}

int ClientConnection::GetId()
{
    return id_;
}

struct RecvResult* ClientConnection::Recv()
{
    char message[1024];
    memset(message, 0, sizeof(message));

    int recv_ret = recv(fd_, message, sizeof(message), 0);

    struct RecvResult* result = new struct RecvResult;

    if (recv_ret < 0)
    {
        result->message = "";
        result->status = RecvStatus::RECV_ERROR;
    }
    else if (recv_ret == 0)
    {
        result->message = "";
        result->status = RecvStatus::CLIENT_EXIT;
    }
    else
    {
        result->message = message;
        result->status = RecvStatus::RECV_SUCCESS;
    }

    return result;
}

bool ClientConnection::Send(const std::string& message)
{
    if (fd_ == -1)
    {
        return false;
    }

    ssize_t send_len = send(fd_, message.c_str(), message.size(), 0);

    if (send_len == -1)
    {
        Close();
        return false;
    }

    return true;
}

void ClientConnection::Close()
{
    if (fd_ != -1)
    {
        int fd = fd_;
        fd_ = -1;
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
}