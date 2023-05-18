#ifndef UZMQ_ENV_H
#define UZMQ_ENV_H


#include "uzmq/platform.h"
#include <boost/property_tree/ptree.hpp>

namespace uzmq{

class UZMQAPI Environment
{
public:
    template<class T>
    static inline T Variable(const std::string& name, const T& default_value=T())
    {
        return cProperties().get<T>(name,default_value);
    }

    static inline std::string Variable(const std::string& name, const std::string& default_value=std::string())
    {
        return cProperties().get<std::string>(name,default_value);
    }

    static std::string SourcePath();

    template<class T>
    static inline void Update(const std::string& name, const T& value,bool saved=true)
    {
        properties().put<T>(name,value);
        if(saved)
        {
           Save();
        }
    }

    static void Preset(std::istringstream& stream);


private:
    static const boost::property_tree::ptree& cProperties();

    static  boost::property_tree::ptree& properties();

    static void Save();
};


}

#define PX_ENV  uzmq::Environment::Variable

#define PX_SET_ENV uzmq::Environment::Update

#define PX_ENV_PATH() uzmq::Environment::SourcePath()

#define PX_PRESET_ENV uzmq::Environment::Preset

#endif // UZMQ_ENV_H
