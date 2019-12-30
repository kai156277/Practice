#include <spdlog/common.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
void initlog()
{

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    //    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("%Y/%m/%d %H:%M:%S.%e %^%L %t %v%$");

    std::shared_ptr<spdlog::logger> logger(new spdlog::logger("app_log", {console_sink}));
    spdlog::set_default_logger(logger);
}
int main()
{
    initlog();
    //    SPDLOG_LOGGER_TRACE(spdlog::get("app_log"), "test");
    SPDLOG_TRACE("测：w试");
    SPDLOG_DEBUG("test");
    SPDLOG_INFO("test");
    SPDLOG_WARN("test");
    SPDLOG_ERROR("test");
    SPDLOG_CRITICAL("test");
    return 0;
}
