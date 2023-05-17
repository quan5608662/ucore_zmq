#ifndef PX_ENV_H
#define PX_ENV_H


#include <platform.h>
#include <boost/property_tree/ptree.hpp>

namespace px_zmq{

class PXAPI Environment
{
public:
    template<class T>
    static inline T Variable(const std::string& name, const T& default_value=T())
    {
        return CProperties().get<T>(name,default_value);
    }

    static inline std::string Variable(const std::string& name, const std::string& default_value=std::string())
    {
        return CProperties().get<std::string>(name,default_value);
    }

    static std::string SourcePath();

    template<class T>
    static inline void Update(const std::string& name, const T& value,bool saved=true)
    {
        Properties().put<T>(name,value);
        if(saved)
        {
           Save();
        }
    }

    static void Preset(std::istringstream& stream);


private:
    static const boost::property_tree::ptree& CProperties();

    static  boost::property_tree::ptree& Properties();

    static void Save();
};


}

#define PX_ENV  px_zmq::Environment::Variable

#define PX_SET_ENV px_zmq::Environment::Update

#define PX_ENV_PATH() px_zmq::Environment::SourcePath()

#define PX_PRESET_ENV px_zmq::Environment::Preset

#endif // ENV_H
