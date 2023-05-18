#include "uzmq/application.h"
#include "appinst.h"

namespace uzmq
{

Application::Application(int argc, char *argv[])
{
    AppInst::setArgument(argc, argv);
}

Application::~Application()
{

}

int Application::exec()
{
    return AppInst::exec();
}
}
