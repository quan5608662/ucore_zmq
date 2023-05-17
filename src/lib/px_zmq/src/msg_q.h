#ifndef MSGQ_H
#define MSGQ_H

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <boost/asio.hpp>
#include "object.h"
#include "message.h"

namespace px_zmq
{
class MsgQ
{
public:
    struct HandleMsgType
    {
        std::shared_ptr<zmq::socket_t> socket;
        NoReplyFunc noRelpyFunc = nullptr;
        ReplyFunc RelpyFunc = nullptr;
        std::shared_ptr<boost::asio::thread_pool> thPool;
        bool block = false;
        bool replyFlag = false;
    };

    explicit MsgQ(std::shared_ptr<zmq::context_t> ctx, const int& pubPort, const int& subPort, const std::string& comType = "inproc://");

    virtual ~MsgQ();

    int run(std::function<void(void)> init, std::function<void(void)> uninit);

    int wait();

    int close();

    void publish(const int& numTopic, const std::string& msg);

    ReplyData request(const int& numTopic, const std::string& msg, int timeout = 3000);

    void add(const int &numTopic, NoReplyFunc func, bool block);

    void add(const int &numTopic, ReplyFunc func, bool block);

private:
    ErrorCode recvTimeout(const std::shared_ptr<zmq::socket_t>& socket, std::vector<zmq::message_t>& recv_msgs, int timeout);

    bool sendReady(const std::shared_ptr<zmq::socket_t> &socket, int timeout = 1000);

    int socketSend(const std::shared_ptr<zmq::socket_t>& socket, const std::string& topic, const std::string& msg);

    int msgLoop(std::function<void(void)> init, std::function<void(void)> uninit);

    void handleMsg(zmq::socket_ref socket);

    void runFunc(std::string topic, std::string msg, HandleMsgType handler);

private:
    std::map<std::string, HandleMsgType> mHandler;

    std::shared_ptr<zmq::socket_t> mPub;

    std::atomic_bool mStartFlag;

    std::future<int> mMsgLoopCtrl;

    zmq::poller_t<> mPoller;

    std::weak_ptr<zmq::context_t> mCtx;

    std::mutex mHandlemutex;

    std::mutex mPubMutex;

    std::string mPubAddr;

    std::string mSubAddr;

    int mHightWaterLevel = 20000;

    std::string mComType = "inproc://";

    std::weak_ptr<Object> mObj;
};
}


#endif // MSGQ_H
