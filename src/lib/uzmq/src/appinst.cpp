#include "uzmq/env.h"
#include "appinst.h"
#include "plugin.h"
#include "loggerinst.h"

namespace uzmq{


void AppInst::setArgument(int argc, char* argv[])
{
    entry().parseArgument(argc,argv);
}

int AppInst::exec()
{   
    return entry().run();
}


const boost::property_tree::ptree &AppInst::cProperties()
{
    return entry().mEnv;
}

boost::property_tree::ptree &AppInst::properties()
{
    return entry().mEnv;
}

boost::any AppInst::option(const std::string &key)
{
    return entry().mOptions[key].value();
}

void AppInst::saveEnv()
{
    try
    {
        auto env_ini = boost::any_cast<std::string>(option("env"));
        boost::property_tree::write_ini(env_ini,cProperties());
    }
    catch(std::exception& e)
    {
        PX_ERROR()<<e.what();
    }
}

void AppInst::initEnv(std::istringstream &stream)
{
    entry().initEnviroment(stream);
}

const std::shared_ptr<zmq::context_t> &AppInst::getZmqContextIo()
{
    return entry().mZmpIo;
}

std::shared_ptr<Logger> &AppInst::getDefaultLogger()
{
    return entry().mDefaultLogger;
}

AppInst::AppInst()
{

}

AppInst::~AppInst()
{

}

void AppInst::parseArgument(int argc, char *argv[])
{
    try
    {
        boost::program_options::options_description desc("options");
        desc.add_options()("help,h","help guide")
                ("workspace,w",boost::program_options::value<std::string>(),"plugin dir")
                ("boost_log_setting,bls",boost::program_options::value<std::string>(),"path of boost log configure file")
                ("env,e",boost::program_options::value<std::string>(),"application global settings");

        boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc),mOptions);
        boost::program_options::notify(mOptions);

        for(auto &iter:mOptions)
        {
            PX_INFO()<<iter.first<<"="<<iter.second.as<std::string>();
        }
    }
    catch(boost::program_options::error& e)
    {
        PX_ERROR()<<e.what();
    }
}


void AppInst::initEnviroment()
{
    try
    {
        if(mOptions.count("env"))
        {
            boost::property_tree::ptree env;
            auto ini = mOptions["env"].as<std::string>();
            boost::property_tree::ini_parser::read_ini(ini, env);

            for(auto &group:env)
            {
                for(auto &opt:group.second)
                {
                    auto key = group.first+"."+opt.first;
                    auto value = opt.second.data();
                    mEnv.put(key,value);
                }
            }
        }


    }
    catch (const std::exception& e)
    {
        PX_ERROR()<<e.what();
    }

    mDefaultLogger = LoggerInst::getInstance().createLogger("ucore");

    printEnv();
}

void AppInst::initEnviroment(std::istringstream &stream)
{
    try
    {
        boost::property_tree::ini_parser::read_ini(stream, mEnv);
    }
    catch (const std::exception& e)
    {
        PX_ERROR()<<e.what();
    }

}

int AppInst::run()
{
    initEnviroment();

    mZmpIo = std::make_shared<zmq::context_t>(0);
    if(mZmpIo == nullptr)
    {
        PX_INFO()<<"Failed to new ZmpIo";
        return -1;
    }

    int xSubPort = PX_ENV("proxy.xsub_port", 65530);
    int xPubPort = PX_ENV("proxy.xpub_port", 65531);
    std::string comType = PX_ENV("proxy.com_type",  "inproc://");
    mRouter = std::make_unique<Router>(mZmpIo, xSubPort, xPubPort, comType);
    if(mRouter == nullptr)
    {
        PX_INFO()<<"Failed to new router";
        return -1;
    }
    if(!mRouter->startUp())
    {
        PX_INFO()<<"Failed to start up router";
        return -1;
    }

    std::string dir = (boost::dll::program_location().parent_path().parent_path()/ "plugin/").string();
    if(mOptions.count("workspace"))
    {
        dir = mOptions["workspace"].as<std::string>();
        PX_INFO() << "workspace is specified" << dir;
    }

    if(!scanAndLoad(dir))
    {
        return -1;
    }

    Plugin::getInstance().exec();

    return mRouter->wait();
}

bool AppInst::scanAndLoad(const boost::filesystem::path &dir)
{
    if(!boost::filesystem::exists(dir))
    {
        PX_ERROR() << "workspace not exist:" << dir;
        return false;
    }

    if(!boost::filesystem::is_directory(dir))
    {
        PX_ERROR() << "workspace is an invalid directoy:" << dir;
        return false;
    }

    for (const auto& iter : boost::filesystem::directory_iterator(dir))
    {
        if(!boost::filesystem::is_regular_file(iter.path()))
        {
            continue;
        }

        if(iter.path().extension() != ".px")
        {
            continue;
        }
        load(iter.path());
    }

    return true;
}

bool AppInst::load(const boost::filesystem::path &path)
{
    try
    {
        if(!Plugin::getInstance().install(path))
        {
            return false;
        }

        PX_INFO() << "success load plugin " << path;

        return true;
    }
    catch (const std::exception& e)
    {
        PX_ERROR() << "failed to load plugin " << path << "," << e.what();
    }

    return false;
}

void AppInst::printEnv()
{
    PX_INFO()<<"-------------------------------------";
    for(auto &group:mEnv)
    {
        for(auto &opt:group.second)
        {
            auto key = group.first+"."+opt.first;
            PX_INFO()<<key<<"="<<opt.second.get_value<std::string>();
        }
    }

    PX_INFO()<<"-------------------------------------";
}

AppInst &AppInst::entry()
{
    static AppInst entry;
    return entry;
}

std::string AppInst::translate(const std::string &input)
{
    static std::regex pattern(".*\\$\\{([A-Za-z0-9_]+)\\}.*");

    static std::smatch matches;
    if(std::regex_match(input,matches,pattern))
    {
        static const auto env = boost::this_process::environment();

        std::string correction(input);
        for(auto &var : matches)
        {
            if(env.count(var))
            {
                correction = boost::replace_first_copy(input, str(boost::format("${%s}")%var),env.at(var).to_string());
            }
        }

        return correction;
    }

    return input;
}

}
