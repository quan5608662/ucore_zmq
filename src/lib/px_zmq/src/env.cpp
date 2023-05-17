#include <env.h>
#include "appinst.h"

namespace px_zmq{


std::string Environment::SourcePath()
{
    return  boost::any_cast<std::string>(AppInst::Option("env"));
}

void Environment::Preset(std::istringstream &stream)
{
    AppInst::InitEnv(stream);
}

const boost::property_tree::ptree &Environment::CProperties()
{
    return AppInst::CProperties();
}

boost::property_tree::ptree &Environment::Properties()
{
    return AppInst::Properties();
}

void Environment::Save()
{
    return AppInst::SaveEnv();
}



};
