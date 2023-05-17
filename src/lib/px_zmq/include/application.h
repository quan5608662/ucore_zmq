#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>

namespace px_zmq
{

class Application
{
public:
    Application(int argc, char* argv[]);

    ~Application();

    static int exec();
};
}

#endif // APPLICATION_H
