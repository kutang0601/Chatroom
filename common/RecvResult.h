#ifndef RECVSTRUCT_H
#define RECVSTRUCT_H

#include <string>

enum class RecvStatus
{
    RECV_SUCCESS,
    RECV_ERROR,
    CLIENT_EXIT,
};

struct RecvResult
{
    RecvStatus status;
    std::string message;
};

#endif