#pragma once

#include "ILogger.h"
#include <array>
#include <mutex>

namespace Components::Logger {

constexpr uint32_t kMaxBufferSize = 32;

class Logger : public ILogger {
public:
  void Info(Category category, const LogMessage&& msg) noexcept final;
  void Warn(Category category, const LogMessage&& msg) noexcept final;
  void Error(Category category, const LogMessage&& msg) noexcept final;
  void Trace(Category category, const LogMessage&& msg) noexcept final;

private:
  void Log(Level level, Category category, const std::string_view msg) noexcept;

  std::array<char, kMaxBufferSize> mTimeBuffer{};
  std::mutex mMutex{};
};
} // namespace Components::Logger