#include "application.h"
#include "appinst.h"

namespace px_zmq
{

Application::Application(int argc, char *argv[])
{
    AppInst::SetArgument(argc, argv);
}

Application::~Application()
{

}

int Application::exec()
{
    return AppInst::Exec();
}
}
