#include "object.h"
#include "msg_q.h"
#include "plugin.h"
#include "appinst.h"
#include "env.h"

namespace px_zmq
{

class Object::Context
{
public:
    explicit Context()
    {
        int xSubPort = PX_ENV("proxy.xsub_port", 65530);
        int xPubPort = PX_ENV("proxy.xpub_port", 65531);
        std::string comType = PX_ENV("proxy.com_type",  "inproc://");
        auto zmqIo = AppInst::GetZmqContextIo();
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

bool Object::Export(const std::shared_ptr<Object>& object)
{
    return Plugin::GetInstance().Add(object);
}


void Object::init()
{

}

void Object::uninit()
{

}

void Object::Publish(const int &topic, const std::string &msg)
{
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        msgQ.lock()->publish(topic, msg);
    }
}

ReplyData Object::Request(const int &topic, const std::string &msg, const int &timeout)
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

void Object::Bind(const int &topic, NoReplyFunc func, bool block)
{
    std::weak_ptr<MsgQ> msgQ = mContext->getMsgQ();
    if(!msgQ.expired())
    {
        return msgQ.lock()->add(topic, func, block);
    }
}

void Object::Bind(const int &topic, ReplyFunc func, bool block)
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

