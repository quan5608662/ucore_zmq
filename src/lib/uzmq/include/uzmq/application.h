#ifndef UZMQ_APPLICATION_H
#define UZMQ_APPLICATION_H

#include <iostream>
#include "platform.h"

namespace uzmq
{

class UZMQAPI Application
{
public:
    Application(int argc, char* argv[]);

    ~Application();

    static int exec();
};
}

#endif // UZMQ_APPLICATION_H
