#include "testb.h"
#include <thread>
#include <boost/asio.hpp>

TestB::TestB()
{
//    connect(5550, &TestB::SubA, this);
//    connect(5551, &TestB::SubB, this);

    std::thread([&]()
    {
        pubA();
    }).detach();

    std::thread([&]()
    {
        pubB();
    }).detach();

    std::thread([&]()
    {
        reqC();
    }).detach();

    std::thread([&]()
    {
        reqD();
    }).detach();
}

TestB::~TestB()
{

}

void TestB::init()
{

}

void TestB::uninit()
{

}

void TestB::pubA()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1000000)
    {
        std::string msg = std::to_string(num++);
        Publish(5550, msg);
        //PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestB::pubB()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1000000)
    {
        std::string msg = std::to_string(num++);
        Publish(5551, msg);
        //PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestB::reqC()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1000000)
    {
        std::string msg = std::to_string(num++);
        auto reply = Request(5552, msg);
        PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestB::reqD()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1000000)
    {
        std::string msg = std::to_string(num++);
        auto reply = Request(5553, msg);
        PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
