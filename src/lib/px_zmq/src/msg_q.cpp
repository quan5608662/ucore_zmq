#include "msg_q.h"

namespace px_zmq
{
MsgQ::MsgQ(std::shared_ptr<zmq::context_t> ctx, const int& pubPort, const int& subPort, const std::string& comType)
    : mCtx(ctx)
    , mComType(comType)
{
    mPubAddr = mComType + std::to_string(pubPort);
    mSubAddr = mComType + std::to_string(subPort);

    mPub = std::make_shared<zmq::socket_t>(*mCtx.lock(), zmq::socket_type::pub);
    mPub->connect(mSubAddr);
    mPub->set(zmq::sockopt::rcvhwm, mHightWaterLevel);
    mPub->set(zmq::sockopt::sndhwm, mHightWaterLevel);

    mHandler.clear();
}

MsgQ::~MsgQ()
{
    close();
}

int MsgQ::run(std::function<void (void)> init, std::function<void (void)> uninit)
{
    if(mMsgLoopCtrl.valid())
    {
        return MSG_LOOP_RUNNING;
    }

    mMsgLoopCtrl = std::async(std::launch::async, &MsgQ::msgLoop, this, init, uninit);

    return NO_ERROR;
}

int MsgQ::wait()
{
    if(false == mMsgLoopCtrl.valid())
    {
        return MSG_LOOP_INVALID;
    }
    return mMsgLoopCtrl.get();
}

int MsgQ::close()
{
    mStartFlag.store(false);
    auto ret = wait();
    mHandler.clear();
    mPub->close();
    mPub.reset();
    return ret;
}


void MsgQ::publish(const int &numTopic, const std::string &msg)
{
    std::unique_lock<std::mutex> lock(mPubMutex);
    std::string topic = std::to_string(numTopic);
    if(mPub)
    {
        socketSend(mPub, topic, msg);
    }
}

ReplyData MsgQ::request(const int &numTopic, const std::string &msg, int timeout)
{
    ReplyData reply;
    std::string topic = std::to_string(numTopic);
    auto socket = std::make_shared<zmq::socket_t>(*mCtx.lock(), zmq::socket_type::req);
    socket->connect(mComType + topic);
    if(socket)
    {
        try
        {
            if(socketSend(socket, topic, msg) != msg.size())
            {
                reply.errorCore = SEND_SIZE_ERROR;
                return reply;
            }
        }
        catch (zmq::error_t e)
        {
            std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
            reply.errorCore = SEND_ERROR;
            return reply;
        }

        std::vector<zmq::message_t> recvMsgs;
        auto ret = recvTimeout(socket, recvMsgs, timeout);
        if(ret != NO_ERROR)
        {
            reply.errorCore = ret;
            return reply;
        }
        reply.errorCore = (topic == recvMsgs.at(0).to_string() ? NO_ERROR : REPLY_NO_MATCH);
        reply.msg = recvMsgs.at(1).to_string();
        return reply;
    }

    return reply;
}

void MsgQ::add(const int &numTopic, NoReplyFunc func, bool block)
{
    try
    {
        std::unique_lock<std::mutex> lock(mHandlemutex);
        std::string topic = std::to_string(numTopic);
        auto iter = mHandler.find(topic);
        if(iter == mHandler.end())
        {
            auto sub = std::make_shared<zmq::socket_t>(*(mCtx.lock()), zmq::socket_type::sub);
            sub->set(zmq::sockopt::subscribe, topic);
            sub->set(zmq::sockopt::rcvhwm, mHightWaterLevel);
            sub->set(zmq::sockopt::sndhwm, mHightWaterLevel);
            sub->connect(mPubAddr);
            std::shared_ptr<boost::asio::thread_pool> pool = nullptr;
            if(false == block)
            {
                pool = std::make_shared<boost::asio::thread_pool>(1);
            }
            mHandler.emplace(topic, HandleMsgType{sub, func, nullptr, pool, block, false});
            mPoller.add(*sub, zmq::event_flags::pollin);
        }
    }
    catch (zmq::error_t e)
    {
        std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
    }
}

void MsgQ::add(const int &numTopic, ReplyFunc func, bool block)
{
    try
    {
        std::unique_lock<std::mutex> lock(mHandlemutex);
        std::string topic = std::to_string(numTopic);
        auto iter = mHandler.find(topic);
        if(iter == mHandler.end())
        {
            auto rep = std::make_shared<zmq::socket_t>(*(mCtx.lock()), zmq::socket_type::rep);
            rep->bind(mComType + topic);
            std::shared_ptr<boost::asio::thread_pool> pool = nullptr;
            if(false == block)
            {
                pool = std::make_shared<boost::asio::thread_pool>(1);
            }
            mHandler.emplace(topic, HandleMsgType{rep, nullptr, func, pool, block, true});
            mPoller.add(*rep, zmq::event_flags::pollin);
        }
    }
    catch (zmq::error_t e)
    {
        std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
    }
}

int MsgQ::msgLoop(std::function<void(void)> init, std::function<void(void)> uninit)
{
    if(init)
    {
        init();
    }
    mStartFlag.store(true);

    if(mHandler.empty())
    {
        std::cout << __FUNCTION__ << "," << __LINE__ << "mHandler is empty:" << std::endl;
        return NO_ERROR;
    }

    int ret = NO_ERROR;
    while(mStartFlag)
    {
        try
        {
            std::vector<zmq::poller_event<>> events;
            events.resize(mHandler.size());
            const std::chrono::milliseconds timeout(1000);
            auto count = mPoller.wait_all(events, timeout);
            if (!count)
            {
                continue;
            }
            else
            {
                for (auto &event : events)
                {
                    if(static_cast<int>(event.events) & static_cast<int>(zmq::event_flags::pollin))
                    {
                        handleMsg(event.socket);
                    }
                }
            }
        }
        catch (zmq::error_t e)
        {
            std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
            ret = MSG_LOOP_EME_STOP;
        }
    }

    if(uninit)
    {
        uninit();
    }

    return ret;
}

ErrorCode MsgQ::recvTimeout(const std::shared_ptr<zmq::socket_t> &socket, std::vector<zmq::message_t>& recvMsgs, int timeout)
{
    zmq::poller_t<> poller;
    std::vector<zmq::poller_event<>> events;
    events.resize(1);
    const std::chrono::milliseconds time(timeout);
    try
    {
        poller.add(*socket, zmq::event_flags::pollin);
        auto count = poller.wait_all(events, time);
        if (!count)
        {
            return REPLY_TIMEOUT;
        }
        else
        {
            for (auto &event : events)
            {
                if(static_cast<int>(event.events) & static_cast<int>(zmq::event_flags::pollin))
                {
                    auto result = zmq::recv_multipart(*socket, std::back_inserter(recvMsgs));
                    if(!result.has_value() || recvMsgs.size() != 2)
                    {
                        std::cout << __FUNCTION__ << "recv msg is error." << std::endl;
                        return RECV_COUNT_ERROR;
                    }
                    else
                    {
                        return NO_ERROR;
                    }
                }
            }
        }
    }
    catch (zmq::error_t e)
    {
        std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
    }

    return RECV_MSGS_ERROR;
}

bool MsgQ::sendReady(const std::shared_ptr<zmq::socket_t> &socket, int timeout)
{
    zmq::poller_t<> poller;
    std::vector<zmq::poller_event<>> events;
    events.resize(1);
    const std::chrono::milliseconds time(timeout);
    try
    {
        poller.add(*socket, zmq::event_flags::pollout);
        auto count = poller.wait_all(events, time);
        if (!count)
        {
            return false;
        }
        else
        {
            for (auto &event : events)
            {
                if(static_cast<int>(event.events) & static_cast<int>(zmq::event_flags::pollout))
                {
                    return true;
                }
            }
        }
    }
    catch (zmq::error_t e)
    {
        std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
    }

    return false;
}

int MsgQ::socketSend(const std::shared_ptr<zmq::socket_t> &socket, const std::string &topic, const std::string &msg)
{
    if(socket && sendReady(socket))
    {
        zmq::message_t dataT(topic);
        zmq::message_t dataM(msg);
        socket->send(dataT, zmq::send_flags::sndmore);
        auto ret = socket->send(dataM, zmq::send_flags::none);
        return ret.value();
    }
    std::cout << __FUNCTION__ << ":send socket is not ready or socket is nullptr." << std::endl;
    return -1;
}

void MsgQ::handleMsg(zmq::socket_ref socket)
{
    std::vector<zmq::message_t> recvMsgs;
    auto result = zmq::recv_multipart(socket, std::back_inserter(recvMsgs));
    if(!result.has_value() || recvMsgs.size() != 2)
    {
        std::cout << __FUNCTION__ << "recv msg is error." << std::endl;
        return;
    }
    //    std::cout << __FUNCTION__ << "[" << recvMsgs[0].to_string() << "] "
    //              << recvMsgs[1].to_string() << std::endl;

    auto topic = recvMsgs.at(0).to_string();
    auto msg = recvMsgs.at(1).to_string();

    auto iter = mHandler.find(topic);
    if(iter != mHandler.end())
    {
        HandleMsgType handler = iter->second;

        if(handler.block && handler.thPool)
        {
            boost::asio::post(*handler.thPool, std::bind(&MsgQ::runFunc, this, topic, msg, handler));
        }
        else
        {
            runFunc(topic, msg, handler);
        }
    }
}

void MsgQ::runFunc(std::string topic, std::string msg, HandleMsgType handler)
{
    if(handler.replyFlag)
    {
        if(handler.RelpyFunc == nullptr)
        {
            return;
        }
        auto replyData = handler.RelpyFunc(msg);
        if(handler.socket)
        {
            socketSend(handler.socket, topic, replyData);
        }
    }
    else
    {
        if(handler.noRelpyFunc == nullptr)
        {
            return;
        }
        handler.noRelpyFunc(msg);
    }
}

}
