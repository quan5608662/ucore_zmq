#include "uzmq/application.h"
#include "uzmq/log.h"
#include "uzmq/env.h"

int main(int argc, char* argv[])
{
    uzmq::Application app(argc,argv);

    return app.exec();
}
