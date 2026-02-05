#include "Logger.h"
#include <chrono>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Components::Logger {
namespace {
constexpr std::string_view CatergoryConvert(Category category) noexcept {
  switch (category) {
  case Category::Init:
    return "Init";
  default:
    return "Unknown";
  }
}

constexpr std::string_view LevelConvert(Level level) noexcept {
  switch (level) {
  case Level::Info:
    return "\033[32mINF\033[0m";
  case Level::Warn:
    return "\033[33mWRN\033[0m";
  case Level::Error:
    return "\033[31mERR\033[0m";
  case Level::Trace:
    return "\033[34mTRC\033[0m";
  default:
    return "\033[31mUNK\033[0m";
  }
}

std::string_view FormatTime(std::array<char, kMaxBufferSize>& buffer,
                            const std::chrono::system_clock::time_point& timePoint) noexcept {
  auto timeT = std::chrono::system_clock::to_time_t(timePoint);
  std::tm tm{};
  localtime_r(&timeT, &tm);

  // Ensure buffer is cleared and use strftime to write into it.
  buffer.fill(0);
  const char* format = "%H:%M:%S";
  std::size_t len = std::strftime(buffer.data(), buffer.size(), format, &tm);
  return std::string_view(buffer.data(), len);
}
} // namespace

void Logger::Info(Category category, const LogMessage&& msg) noexcept {
  Log(Level::Info, category, msg.View());
}

void Logger::Warn(Category category, const LogMessage&& msg) noexcept {
  Log(Level::Warn, category, msg.View());
}

void Logger::Error(Category category, const LogMessage&& msg) noexcept {
  Log(Level::Error, category, msg.View());
}

void Logger::Trace(Category category, const LogMessage&& msg) noexcept {
  Log(Level::Trace, category, msg.View());
}

void Logger::Log(Level level, Category category, const std::string_view msg) noexcept {
  std::lock_guard lock(mMutex);
  auto timeStr = FormatTime(mTimeBuffer, std::chrono::system_clock::now());
  auto levelStr = LevelConvert(level);
  auto categoryStr = CatergoryConvert(category);

  std::cout << "[" << timeStr << "][" << levelStr << "][" << categoryStr << "] " << msg
            << std::endl;
}
} // namespace Components::Logger
