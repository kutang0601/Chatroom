#include "ClientConnection.h"
#include "RecvResult.h"

#include <bits/types/cookie_io_functions_t.h>
#include <cstring>
#include <stdexcept>
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
    if (fd_ != -1)
    {
        close(fd_);
    }
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

void ClientConnection::Send(const std::string& message)
{
    ssize_t send_len = send(fd_, message.c_str(), message.size(), 0);

    if (send_len == -1)
    {
        throw std::runtime_error("send fail!");
    }
}