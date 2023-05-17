#include <application.h>
#include <log.h>
#include <env.h>

int main(int argc, char* argv[])
{
    px_zmq::Application app(argc,argv);

    return app.exec();
}
