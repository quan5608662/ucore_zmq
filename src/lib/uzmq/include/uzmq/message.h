#ifndef UZMQ_MESSAGE_H
#define UZMQ_MESSAGE_H

#include <iostream>
#include <functional>

namespace uzmq
{
enum ErrorCode
{
    NO_ERROR = 0,
    MSGQ_IS_NULLPTR,
    MSG_LOOP_RUNNING,
    MSG_LOOP_INVALID,
    MSG_LOOP_EME_STOP,
    SOCKET_NULL_ERROR,
    SEND_SIZE_ERROR,
    SEND_ERROR,
    REPLY_TIMEOUT,
    REPLY_NO_MATCH,
    RECV_COUNT_ERROR,
    RECV_MSGS_ERROR,
};

struct ReplyData
{
    ErrorCode errorCore;
    std::string msg;
};

using ReplyFunc = std::function<std::string(const std::string&)>;
using NoReplyFunc = std::function<void(const std::string&)>;
}


#endif // MESSAGE_H
