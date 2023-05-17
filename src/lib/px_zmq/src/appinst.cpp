#include "appinst.h"
#include "env.h"
#include "plugin.h"

namespace px_zmq{


void AppInst::SetArgument(int argc, char* argv[])
{
    Entry().ParseArgument(argc,argv);
}

int AppInst::Exec()
{   
    return Entry().Run();
}


const boost::property_tree::ptree &AppInst::CProperties()
{
    return Entry().env_;
}

boost::property_tree::ptree &AppInst::Properties()
{
    return Entry().env_;
}

boost::any AppInst::Option(const std::string &key)
{
    return Entry().options_[key].value();
}

void AppInst::SaveEnv()
{
    try
    {
        auto env_ini = boost::any_cast<std::string>(Option("env"));
        boost::property_tree::write_ini(env_ini,CProperties());
    }
    catch(std::exception& e)
    {
        PX_ERROR()<<e.what();
    }
}

void AppInst::InitEnv(std::istringstream &stream)
{
    Entry().InitEnviroment(stream);
}

const std::shared_ptr<zmq::context_t> &AppInst::GetZmqContextIo()
{
    return Entry().mZmpIo;
}

AppInst::AppInst()
{

}

AppInst::~AppInst()
{

}

void AppInst::ParseArgument(int argc, char *argv[])
{
    try
    {
        boost::program_options::options_description desc("options");
        desc.add_options()("help,h","help guide")
                ("workspace,w",boost::program_options::value<std::string>(),"plugin dir")
                ("boost_log_setting,bls",boost::program_options::value<std::string>(),"path of boost log configure file")
                ("env,e",boost::program_options::value<std::string>(),"application global settings");

        boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc),options_);
        boost::program_options::notify(options_);

        for(auto &iter:options_)
        {
            PX_INFO()<<iter.first<<"="<<iter.second.as<std::string>();
        }
    }
    catch(boost::program_options::error& e)
    {
        PX_ERROR()<<e.what();
    }
}


void AppInst::InitEnviroment()
{
    try
    {
        if(options_.count("env"))
        {
            boost::property_tree::ptree env;
            auto ini = options_["env"].as<std::string>();
            boost::property_tree::ini_parser::read_ini(ini, env);

            for(auto &group:env)
            {
                for(auto &opt:group.second)
                {
                    auto key = group.first+"."+opt.first;
                    auto value = opt.second.data();
                    env_.put(key,value);
                }
            }
        }


    }
    catch (const std::exception& e)
    {
        PX_ERROR()<<e.what();
    }


    Logger::Init();

    PrintEnv();
}

void AppInst::InitEnviroment(std::istringstream &stream)
{
    try
    {
        boost::property_tree::ini_parser::read_ini(stream, env_);
    }
    catch (const std::exception& e)
    {
        PX_ERROR()<<e.what();
    }

}

int AppInst::Run()
{
    InitEnviroment();

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
    if(options_.count("workspace"))
    {
        dir = options_["workspace"].as<std::string>();
        PX_INFO() << "workspace is specified" << dir;
    }

    if(!ScanAndLoad(dir))
    {
        return -1;
    }

    Plugin::GetInstance().Exec();

    return mRouter->wait();
}

bool AppInst::ScanAndLoad(const boost::filesystem::path &dir)
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
        Load(iter.path());
    }

    return true;
}

bool AppInst::Load(const boost::filesystem::path &path)
{
    try
    {
        if(!Plugin::GetInstance().Install(path))
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

void AppInst::PrintEnv()
{
    PX_INFO()<<"-------------------------------------";
    for(auto &group:env_)
    {
        for(auto &opt:group.second)
        {
            auto key = group.first+"."+opt.first;
            PX_INFO()<<key<<"="<<opt.second.get_value<std::string>();
        }
    }

    PX_INFO()<<"-------------------------------------";
}

AppInst &AppInst::Entry()
{
    static AppInst entry;
    return entry;
}

std::string AppInst::Translate(const std::string &input)
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
