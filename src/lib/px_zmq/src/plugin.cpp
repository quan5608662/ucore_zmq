#include "plugin.h"
#include "appinst.h"

namespace px_zmq
{
Plugin::Plugin()
{

}

Plugin::~Plugin()
{

}


Plugin &Plugin::GetInstance()
{
    static Plugin instance;
    return instance;
}

bool Plugin::Install(const boost::filesystem::path &path)
{
    boost::dll::shared_library lib(path);

    if(!lib.is_loaded())
    {
        return false;
    }

    if(!lib.has("__main__"))
    {
        return false;
    }

    mDlls[path] = lib;

    auto& symbol = lib.get<bool()>("__main__");

    return symbol();
}


bool Plugin::Add(const std::shared_ptr<Object>& obj)
{
    if(!obj)
    {
        return false;
    }
    auto name = boost::typeindex::type_id_runtime(*obj).pretty_name();
    mObjs[name] = obj;
    return true;
}

int Plugin::Exec()
{
    for(auto iter = mObjs.begin(); iter != mObjs.end(); iter++)
    {
        std::weak_ptr<Object> obj = iter->second;
        if(!obj.expired())
        {
            obj.lock()->startUp();
        }
    }
    return 0;
}
}
