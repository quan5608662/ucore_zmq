#ifndef PX_LOGGER_H
#define PX_LOGGER_H

#include <boost/filesystem.hpp>
#include "log.h"


namespace px_zmq{

class Logger
{
public:
    Logger();

    static void Init();
protected:
    static void Init(const boost::filesystem::path& filename, const boost::filesystem::path& folder,const std::string& level);

    static void Configure(const boost::filesystem::path& setting);
};

}



#endif // LOGGER_H
