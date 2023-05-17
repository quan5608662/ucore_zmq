#ifndef TESTA_H
#define TESTA_H

#include "object.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

class TestA: public px_zmq::Object
{
public:
    TestA();

    ~TestA();

    void init() override;

    void uninit() override;

    void SubA(const std::string& msg);

    void SubB(const std::string& msg);

    std::string RepC(const std::string& msg);

    std::string RepD(const std::string& msg);

private:
    std::atomic_int numA;
    std::atomic_int  numB;
    std::atomic_int  numC;
    std::atomic_int  numD;

};

#endif // TESTA_H
