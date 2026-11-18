#pragma once
#include "defines.h"
#include <format>
#include <source_location>

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if POVLY_RELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

namespace povly::logging {
    enum class Level { Fatal, Error, Warn, Info, Debug, Trace };

    void init();
    void shutdown();

    template <typename... Args>
    void log(Level level, std::format_string<Args...> fmt, Args &&...args);
    // const std::source_location &loc = std::source_location::current()
} // namespace povly::logging
