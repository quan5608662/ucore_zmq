#include "testa.h"
#include <thread>

TestA::TestA()
{
    connect(5550, &TestA::SubA, this);
    connect(5551, &TestA::SubB, this);

    connect(5552, &TestA::RepC, this);
    connect(5553, &TestA::RepD, this);

    numA.store(0);
    numB.store(0);
    numC.store(0);
    numD.store(0);

    std::thread([&]()
    {
        while(1)
        {
            PX_LOG_INFO()  << __FUNCTION__ << "Count:" << numA << "," << numB << "," << numC << "," << numD ;
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        }
    }).detach();
}

TestA::~TestA()
{

}

void TestA::init()
{
    PX_LOG_INFO()  << __FUNCTION__ << "1111111111111111111111111111111111111";
}

void TestA::uninit()
{

}

void TestA::SubA(const std::string &msg)
{
    numA++;
    PX_LOG_INFO()  << __FUNCTION__ << "SubA:" << msg << "," << numA << "," << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void TestA::SubB(const std::string &msg)
{
    numB++;
    PX_LOG_INFO()  << __FUNCTION__ << "SubB:" << msg << "," << numB << "," << std::this_thread::get_id() << std::endl;
}


std::string TestA::RepC(const std::string& msg)
{
    PX_LOG_INFO()  << __FUNCTION__ << "RepC:" << msg << "," << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    numC++;
    return "repC:" + std::to_string(numC.load());
}

std::string TestA::RepD(const std::string& msg)
{
    PX_LOG_INFO()  << __FUNCTION__ << "RepD:" << msg << "," << std::this_thread::get_id() << std::endl;
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    numD++;
    return "repD:" + std::to_string(numC.load());
}
