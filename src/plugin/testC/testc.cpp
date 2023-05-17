#include "testc.h"
#include <thread>
#include <boost/asio.hpp>

TestC::TestC()
{
//    connect(5550, &TestC::SubA, this);
//    connect(5551, &TestC::SubB, this);

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

TestC::~TestC()
{

}

void TestC::init()
{

}

void TestC::uninit()
{

}

void TestC::pubA()
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

void TestC::pubB()
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

void TestC::reqC()
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

void TestC::reqD()
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
