#include "uzmq/env.h"
#include "uzmq/object.h"
#include "msgq.h"
#include "plugin.h"
#include "appinst.h"

namespace uzmq
{

class Object::Context
{
public:
    explicit Context()
    {
        int xSubPort = PX_ENV("proxy.xsub_port", 65530);
        int xPubPort = PX_ENV("proxy.xpub_port", 65531);
        std::string comType = PX_ENV("proxy.com_type",  "inproc://");
        auto zmqIo = AppInst::getZmqContextIo();
        mMsgQ = std::make_shared<MsgQ>(zmqIo, xSubPort, xPubPort, comType);
    }

    virtual ~Context()
    {

    }

    const std::shared_ptr<MsgQ>& getMsgQ()
    {
        return mMsgQ;
    }


private:
    std::shared_ptr<MsgQ> mMsgQ;
};


Object::Object(): mContext(std::make_unique<Context>())
{

}

Object::~Object()
{

}

bool Object::exportObj(const std::shared_ptr<Object>& object)
{
    return Plugin::getInstance().add(object);
}


void Object::init()
{

}

void Object::uninit()
{

}

void Object::publish(const int &topic, const std::string &msg)
{
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        msgQ.lock()->publish(topic, msg);
    }
}

ReplyData Object::request(const int &topic, const std::string &msg, const int &timeout)
{
    ReplyData data;
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        return msgQ.lock()->request(topic, msg, timeout);
    }
    data.errorCore = MSGQ_IS_NULLPTR;
    return data;
}

void Object::bind(const int &topic, NoReplyFunc func, bool block)
{
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        return msgQ.lock()->add(topic, func, block);
    }
}

void Object::bind(const int &topic, ReplyFunc func, bool block)
{
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        return msgQ.lock()->add(topic, func, block);
    }
}

void Object::startUp()
{
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        msgQ.lock()->run(std::bind(&Object::init, this), std::bind(&Object::uninit, this));
    }
}
}

