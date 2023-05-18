#ifndef UZMQ_LOG_H
#define UZMQ_LOG_H

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <vector>

#define TARGET_LOG_DEBUG(logger) BOOST_LOG_SEV(logger, boost::log::trivial::debug) <<BOOST_CURRENT_FUNCTION <<"|"
#define TARGET_LOG_INFO(logger)  BOOST_LOG_SEV(logger, boost::log::trivial::info) <<BOOST_CURRENT_FUNCTION<<"|"
#define TARGET_LOG_WARN(logger)  BOOST_LOG_SEV(logger, boost::log::trivial::warning) <<BOOST_CURRENT_FUNCTION<<"|"
#define TARGET_LOG_ERROR(logger) BOOST_LOG_SEV(logger, boost::log::trivial::error) <<BOOST_CURRENT_FUNCTION<<"|"

#define PX_LOG_DEBUG()   TARGET_LOG_DEBUG(uzmq::defaultLogger())
#define PX_LOG_INFO()    TARGET_LOG_INFO(uzmq::defaultLogger())
#define PX_LOG_WARN()    TARGET_LOG_WARN(uzmq::defaultLogger())
#define PX_LOG_ERROR()   TARGET_LOG_ERROR(uzmq::defaultLogger())

#define PX_DEBUG PX_LOG_DEBUG
#define PX_INFO  PX_LOG_INFO
#define PX_WARN  PX_LOG_WARN
#define PX_ERROR PX_LOG_ERROR

#ifndef LOG_DEBUG
#define LOG_DEBUG PX_DEBUG
#endif

#ifndef LOG_INFO
#define LOG_INFO PX_INFO
#endif

#ifndef LOG_WARN
#define LOG_WARN PX_WARN
#endif

#ifndef LOG_ERROR
#define LOG_ERROR PX_ERROR
#endif

boost::log::record_ostream& operator<<(boost::log::record_ostream &os, std::vector<int> &value);
boost::log::record_ostream& operator<<(boost::log::record_ostream &os, std::vector<float> &value);
boost::log::record_ostream& operator<<(boost::log::record_ostream &os, std::vector<int64_t> &value);
boost::log::record_ostream& operator<<(boost::log::record_ostream &os, std::vector<std::string> &value);


namespace uzmq
{
    using Logger = boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level, std::string>;

    Logger& defaultLogger();

    std::shared_ptr<Logger> creatLogger(const std::string& moduleName);
}



#endif // UZMQ_LOG_H
