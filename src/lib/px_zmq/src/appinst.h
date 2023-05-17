#ifndef PX_APPINST_H
#define PX_APPINST_H

#include "logger.h"
#include "router.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/process.hpp>
#include <regex>
#include <zmq.hpp>
#include <msg_q.h>

namespace px_zmq
{


class AppInst
{
public:
    static void SetArgument(int argc, char* argv[]);

    static int Exec();

    static const boost::property_tree::ptree& CProperties();

    static  boost::property_tree::ptree& Properties();

    static boost::any Option(const std::string& key);

    static void SaveEnv();

    static void InitEnv(std::istringstream& stream);

    static const std::shared_ptr<zmq::context_t>& GetZmqContextIo();

protected:
    AppInst();

    ~AppInst();


    void ParseArgument(int argc, char* argv[]);

    void InitEnviroment();

    void InitEnviroment(std::istringstream& stream);

    int Run();

    void PrintEnv();
private:
    static AppInst& Entry();

    static std::string Translate(const std::string& value);

private:
    bool ScanAndLoad(const boost::filesystem::path &dir);

    bool Load(const boost::filesystem::path &path);

private:
    Logger logger_;

    std::unique_ptr<Router> mRouter =nullptr;

    std::shared_ptr<zmq::context_t> mZmpIo = nullptr;

    boost::property_tree::ptree env_;

    boost::program_options::variables_map options_;
};

}

#endif // APPINST_H
