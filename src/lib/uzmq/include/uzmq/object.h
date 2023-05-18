#ifndef UZMQ_OBJECT_H
#define UZMQ_OBJECT_H

#include "log.h"
#include "message.h"
#include "platform.h"
#include <boost/format.hpp>

namespace uzmq
{
class  UZMQAPI Object
{
public:
    explicit Object();

    virtual ~Object();

    template<class T>
    static bool exportObj()
    {
        return exportObj(std::make_shared<T>());
    }
    static bool exportObj(const std::shared_ptr<Object>& object);

protected:
    virtual void init();

    virtual void uninit();
public:
    template<class T>
    void connect(const int& topic,  void (T::*fn)(const std::string&), T* obj, bool block = false)
    {
        obj->bind(topic, std::bind(fn, obj, std::placeholders::_1), block);
    }

    template<class T>
    void connect(const int& topic,  std::string (T::*fn)(const std::string&), T* obj, bool block = false)
    {
        obj->bind(topic, ReplyFunc(std::bind(fn, obj, std::placeholders::_1)), block);
    }

    void publish(const int& topic, const std::string& msg = "");

    ReplyData request(const int& topic, const std::string& msg = "", const int& timeout = 5000);

    void bind(const int& topic, NoReplyFunc func, bool block);

    void bind(const int& topic, ReplyFunc func, bool block);

    void startUp();

    void disconnect(const int& topic);

private:
    class Context;
    std::unique_ptr<Context> mContext;
};



#define DECLARE_PLUGIN() \
extern "C" UZMQAPI bool __main__(){
#define OBJECT_EXPORT(type) \
    if(!uzmq::Object::exportObj<type>()) return false;
#define DECLARE_PLUGIN_END()  return true;}

}
#endif // UZMQ_OBJECT_H
