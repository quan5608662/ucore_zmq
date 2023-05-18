#ifndef TESTB_H
#define TESTB_H

#include "uzmq/object.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

class TestB: public uzmq::Object
{
public:
    TestB();

    ~TestB();

    void init() override;

    void uninit() override;

    void pubA();

    void pubB();

    void reqC();

    void reqD();

private:
    int numA = 0;
    int numB = 0;
    int numC = 0;
    int numD = 0;

};

#endif // TESTB_H
