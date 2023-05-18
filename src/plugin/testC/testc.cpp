#include "testc.h"
#include <thread>
#include <boost/asio.hpp>

TestC::TestC()
{
    connect(5550, &TestC::SubA, this);
    connect(5551, &TestC::SubB, this);

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
    while(num < 1)
    {
        std::string msg = std::to_string(num++);
        publish(5550, msg);
        //PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg ;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestC::pubB()
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

void TestC::reqC()
{
    int num = 0;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    while(num < 1)
    {
        std::string msg = std::to_string(num++);
        auto reply = request(5552, msg);
        PX_LOG_INFO()  << "Replay error, msg:"  << reply.errorCore << "," << reply.msg ;
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void TestC::reqD()
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

void TestC::SubA(const std::string &msg)
{
    numA++;
    PX_LOG_INFO()  << __FUNCTION__ << "SubA:" << msg.size() << "," << numA << "," << std::this_thread::get_id();
    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void TestC::SubB(const std::string &msg)
{
    numB++;
    PX_LOG_INFO()  << __FUNCTION__ << "SubB:" << msg << "," << numB << "," << std::this_thread::get_id();
}
