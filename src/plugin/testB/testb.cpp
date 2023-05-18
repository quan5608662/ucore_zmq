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
    while(num < 10)
    {
        std::string msg = std::to_string(num++);
        std::string largeString;
        int size = 200*1024*1024; // 100 million
        for (int i = 1; i <= size; ++i)
        {
            largeString += "a";
        }
        PX_LOG_INFO()  << "111111111111111111111111111111111111111111111";
        publish(5550, largeString);
        //PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg ;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestB::pubB()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1)
    {
        std::string msg = std::to_string(num++);
        publish(5551, msg);
        //PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg ;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestB::reqC()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 10)
    {
        std::string msg = std::to_string(num++);
        std::string largeString;
        int size = 200*1024*1024; // 100 million
        for (int i = 1; i <= size; ++i)
        {
            largeString += "a";
        }
        PX_LOG_INFO()  << "111111111111111111111111111111111111111111111";
        auto reply = request(5552, largeString);
        PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg ;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestB::reqD()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1)
    {
        std::string msg = std::to_string(num++);
        auto reply = request(5553, msg);
        PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg ;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
