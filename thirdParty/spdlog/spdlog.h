#pragma once

namespace spdlog {

#define SPDLOG_LEVEL_TRACE 0
#define SPDLOG_LEVEL_DEBUG 1
#define SPDLOG_LEVEL_INFO 2
#define SPDLOG_LEVEL_WARN 3
#define SPDLOG_LEVEL_ERROR 4
#define SPDLOG_LEVEL_CRITICAL 5
#define SPDLOG_LEVEL_OFF 6


namespace level {

enum level_enum : int {

    trace = SPDLOG_LEVEL_TRACE,
    debug = SPDLOG_LEVEL_DEBUG,
    info = SPDLOG_LEVEL_INFO,
    warn = SPDLOG_LEVEL_WARN,
    err = SPDLOG_LEVEL_ERROR,
    critical = SPDLOG_LEVEL_CRITICAL,
    off = SPDLOG_LEVEL_OFF,
    n_levels
};

}

inline void set_level(level::level_enum log_level)
{

}

#define SPDLOG_TRACE(...) (void)0
#define SPDLOG_DEBUG(...) (void)0
#define SPDLOG_INFO(...) (void)0
#define SPDLOG_ERROR(...) (void)0
#define SPDLOG_CRITICAL(...) (void)0

}