#include "Test.h"

#include "TestSpdlog.h"

Test::Test()
{
    SPDLOG_TRACE("Trace spdlog Test");
    SPDLOG_DEBUG("Debug spdlog Test");
    SPDLOG_INFO("Info spdlog Test");
    SPDLOG_WARN("Warn spdlog Test");
    SPDLOG_ERROR("Error spdlog Test");
    SPDLOG_CRITICAL("Critical spdlog Test");
}
