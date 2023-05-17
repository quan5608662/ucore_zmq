#ifndef OBJECT_H
#define OBJECT_H

#include "log.h"
#include "message.h"
#include "platform.h"
#include <boost/format.hpp>

namespace px_zmq
{
class MsgQ;
class  PXAPI Object
{
public:
    explicit Object();

    virtual ~Object();

    template<class T>
    static bool Export()
    {
        return Export(std::make_shared<T>());
    }
    static bool Export(const std::shared_ptr<Object>& object);

protected:
    virtual void init();

    virtual void uninit();
public:
    template<class T>
    void connect(const int& topic,  void (T::*fn)(const std::string&), T* obj, bool block = false)
    {
        obj->Bind(topic, std::bind(fn, obj, std::placeholders::_1), block);
    }

    template<class T>
    void connect(const int& topic,  std::string (T::*fn)(const std::string&), T* obj, bool block = false)
    {
        obj->Bind(topic, ReplyFunc(std::bind(fn, obj, std::placeholders::_1)), block);
    }

    void Publish(const int& topic, const std::string& msg = "");

    ReplyData Request(const int& topic, const std::string& msg = "", const int& timeout = 5000);

    void Bind(const int& topic, NoReplyFunc func, bool block);

    void Bind(const int& topic, ReplyFunc func, bool block);

    void startUp();

    void Disconnect(const int& topic);

private:
    class Context;
    std::unique_ptr<Context> mContext;
};



#define DECLARE_PLUGIN() \
extern "C" PXAPI bool __main__(){
#define OBJECT_EXPORT(type) \
    if(!px_zmq::Object::Export<type>()) return false;
#define DECLARE_PLUGIN_END()  return true;}

}
#endif // OBJECT_H
