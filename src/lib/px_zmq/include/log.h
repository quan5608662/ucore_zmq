#ifndef PX_LOG_H
#define PX_LOG_H

#include <boost/log/trivial.hpp>


#define PX_LOG_DEBUG()   BOOST_LOG_TRIVIAL(debug)<<BOOST_CURRENT_FUNCTION<<"|"
#define PX_LOG_INFO()    BOOST_LOG_TRIVIAL(info)<<BOOST_CURRENT_FUNCTION<<"|"
#define PX_LOG_WARN()    BOOST_LOG_TRIVIAL(warning)<<BOOST_CURRENT_FUNCTION<<"|"
#define PX_LOG_ERROR()   BOOST_LOG_TRIVIAL(error)<<BOOST_CURRENT_FUNCTION<<"|"

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


#endif // LOG_H
