#include <boost/exception/all.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <string>

#ifdef BOOST_LOG_STREAM_WITH_PARAMS
#    pragma message("boost_log_stream_with_params")
#endif

enum severity_level
{
    normal = 0,
    notification,
    warning,
    error,
    critical
};

std::ostream &operator<<(std::ostream &out, severity_level level)
{
    static const std::string level_str[5] = {"normal", "notification", "warning", "error", "critical"};
    out << level_str[level];
    return out;
}

namespace src = boost::log::sources;

int main()
{
    std::cout << "test:" << normal << std::endl;
    src::severity_logger<severity_level> slg;
    BOOST_LOG_SEV(slg, normal) << "A regular message";
    BOOST_LOG_SEV(slg, warning) << "Something bad is going on but I can handle it";
    BOOST_LOG_SEV(slg, critical) << "Everything crumbles, shoot me now!";
    BOOST_LOG_TRIVIAL(info) << "Test";
    return 0;
}
