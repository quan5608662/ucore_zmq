#ifndef PX_PLUGIN_H
#define PX_PLUGIN_H

#include <object.h>
#include <boost/dll.hpp>
#include <boost/type_index.hpp>
#include <log.h>
#include <map>


namespace px_zmq{


class Plugin
{
public:
    static Plugin& GetInstance();

    bool Install(const boost::filesystem::path& path);

    bool Add(const std::shared_ptr<Object>& obj);

    int Exec();

private:
    std::map<boost::filesystem::path, boost::dll::shared_library> mDlls;

    std::map<std::string,std::shared_ptr<Object>> mObjs;

private:
    Plugin(const Plugin &) = delete;
    Plugin &operator=(const Plugin &) = delete;
    Plugin();
    ~Plugin();
};

}
#endif // PLUGIN_H
