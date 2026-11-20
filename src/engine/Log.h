#pragma once

#include <memory>

#include "engine/Core.h"
#include "spdlog/spdlog.h"

namespace Povly {
class POVLY_API Log {
 public:
  static void Init();

  static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
    return s_CoreLogger;
  }

  static std::shared_ptr<spdlog::logger>& GetClientLogger() {
    return s_ClientLogger;
  }

 private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}  // namespace Povly

// Core log Macros
#define POVLY_CORE_TRACE(...) ::Povly::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define POVLY_CORE_INFO(...) ::Povly::Log::GetCoreLogger()->info(__VA_ARGS__)
#define POVLY_CORE_WARN(...) ::Povly::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define POVLY_CORE_ERROR(...) ::Povly::Log::GetCoreLogger()->error(__VA_ARGS__)
#define POVLY_CORE_FATAL(...) ::Povly::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log Macros
#define POVLY_CLIENT_TRACE(...) \
  ::Povly::Log::GetClientLogger()->trace(__VA_ARGS__)
#define POVLY_CLIENT_INFO(...) \
  ::Povly::Log::GetClientLogger()->info(__VA_ARGS__)
#define POVLY_CLIENT_WARN(...) \
  ::Povly::Log::GetClientLogger()->warn(__VA_ARGS__)
#define POVLY_CLIENT_ERROR(...) \
  ::Povly::Log::GetClientLogger()->error(__VA_ARGS__)
#define POVLY_CLIENT_FATAL(...) \
  ::Povly::Log::GetClientLogger()->fatal(__VA_ARGS__)
