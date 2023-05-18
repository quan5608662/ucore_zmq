#ifndef UZMQ_ROUTER_H
#define UZMQ_ROUTER_H

#include <zmq.hpp>
#include <zmq_addon.hpp>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <future>


class Router
{
public:
    explicit Router(std::shared_ptr<zmq::context_t> ctx, const int& pubPort, const int& subPort, const std::string& comType = "inproc://");

    virtual ~Router();

    bool startUp();

    bool wait();
private:

    std::weak_ptr<zmq::context_t> mCtx;

    std::string mComType = "inproc://";

    std::string mPubAddr;

    std::string mSubAddr;

    std::unique_ptr<zmq::socket_t> mXSub;

    std::unique_ptr<zmq::socket_t> mXPub;

    std::future<bool> mProxyCtrl;

    int mHightWaterLevel = 20000;
};

#endif // ROUTER_H
