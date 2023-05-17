#include "router.h"

Router::Router(std::shared_ptr<zmq::context_t> ctx, const int& pubPort, const int& subPort, const std::string& comType)
    : mCtx(ctx)
    , mComType(comType)
{
    mPubAddr = mComType + std::to_string(pubPort);
    mSubAddr = mComType + std::to_string(subPort);
}

Router::~Router()
{
    mXSub->close();
    mXPub->close();
    wait();
    std::cout << "release proxy done" << std::endl;
}

bool Router::startUp()
{
    std::promise<bool> result;
    mProxyCtrl = std::async(std::launch::async, [&]()
    {
        try
        {
            std::cout << "Starting proxy..." << std::endl;

            // Create mXSub XSUB socket
            mXSub = std::make_unique<zmq::socket_t>(*mCtx.lock(), zmq::socket_type::xsub);
            mXSub->bind(mSubAddr);

            // Create mXPub XPUB socket
            mXPub = std::make_unique<zmq::socket_t>(*mCtx.lock(), zmq::socket_type::xpub);
            mXPub->bind(mPubAddr);

            mXPub->set(zmq::sockopt::rcvhwm, mHightWaterLevel);
            mXPub->set(zmq::sockopt::sndhwm, mHightWaterLevel);
            mXSub->set(zmq::sockopt::rcvhwm, mHightWaterLevel);
            mXSub->set(zmq::sockopt::sndhwm, mHightWaterLevel);

            result.set_value(true);
            // Start the proxy
            zmq::proxy(*mXSub, *mXPub);
            // never reach here
        }
        catch(zmq::error_t e)
        {
            result.set_value(false);
            std::cout << __FUNCTION__ << ":catch error:" << e.what() << std::endl;
        }
        return false;
    });
    return result.get_future().get();
}

bool Router::wait()
{
    if(false == mProxyCtrl.valid())
    {
        return -1;
    }

    return mProxyCtrl.get();
}
