#ifndef TESTB_H
#define TESTB_H

#include "object.h"
#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

class TestC: public px_zmq::Object
{
public:
    TestC();

    ~TestC();

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
