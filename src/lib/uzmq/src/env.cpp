#include "uzmq/env.h"
#include "appinst.h"

namespace uzmq{


std::string Environment::SourcePath()
{
    return  boost::any_cast<std::string>(AppInst::option("env"));
}

void Environment::Preset(std::istringstream &stream)
{
    AppInst::initEnv(stream);
}

const boost::property_tree::ptree &Environment::cProperties()
{
    return AppInst::cProperties();
}

boost::property_tree::ptree &Environment::properties()
{
    return AppInst::properties();
}

void Environment::Save()
{
    return AppInst::saveEnv();
}



};
