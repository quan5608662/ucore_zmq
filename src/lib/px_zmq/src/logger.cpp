#include "logger.h"

#include <env.h>
#include <log.h>

#include <fstream>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/utility/setup/settings.hpp>
#include <boost/log/utility/setup/from_settings.hpp>
#include <boost/dll.hpp>

namespace px_zmq{

Logger::Logger()
{
    boost::log::add_common_attributes();
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::register_simple_filter_factory<boost::log::trivial::severity_level, char>("Severity");
}

void Logger::Init()
{
    auto app = boost::dll::program_location().filename();
    auto folder = PX_ENV("log.dir",boost::dll::program_location().parent_path()/"log");
    auto level = PX_ENV("log.level","trace");

    Init(app,folder,level);

    auto settings = PX_ENV("log.settings");

    if(!settings.empty())
    {
        Configure(settings);
    }

}

void Logger::Init(const boost::filesystem::path& filename, const boost::filesystem::path& folder,const std::string& level)
{
    boost::log::settings settings;
    settings["Core"]["Filter"] = "%Severity% >= "+level;
    settings["Core"]["DisableLogging"] = false;


    settings["Sinks.Console"]["Destination"] = "Console";
    settings["Sinks.Console"]["Format"] = "[%TimeStamp%] [%Severity%] %Message%";
    settings["Sinks.Console"]["Filter"] = "%Severity% >= trace";
    settings["Sinks.Console"]["AutoFlush"] = true;


    settings["Sinks.File"]["Destination"] = "TextFile";
    settings["Sinks.File"]["Format"] = "[%TimeStamp%] [%Severity%] %Message%";
    settings["Sinks.File"]["FileName"] = (folder/filename).string()+"_%Y%m%d_%H%M%S_%3N.log";
    settings["Sinks.File"]["Target"] = folder.string();
    settings["Sinks.File"]["Filter"] = "%Severity% >= trace";
    settings["Sinks.File"]["AutoFlush"] = true;
    settings["Sinks.File"]["RotationSize"] = 10 * 1024 * 1024; // 10 MiB
    settings["Sinks.File"]["ScanForFiles"] = "Matching";
    settings["Sinks.File"]["MaxSize"] = 1000 * 1024 * 1024;  //200 MiB
    settings["Sinks.File"]["RotationTimePoint"] = "00:00:00";
    boost::log::init_from_settings(settings);
}

void Logger::Configure(const boost::filesystem::path &setting)
{
    try
    {
        std::ifstream ifs(setting.string());
        boost::log::init_from_stream(ifs);
    }
    catch (const std::exception& e)
    {
        PX_LOG_ERROR()<<e.what();
    }
}



}
