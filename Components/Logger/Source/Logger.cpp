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
  case Category::Thread:
    return "Thread";
  default:
    return "Unknown";
  }
}

constexpr std::size_t LongestCategoryLength() noexcept {
  std::size_t maxLength = 0;
  for (uint8_t i = 0; i < static_cast<uint8_t>(Category::COUNT); ++i) {
    auto length = CatergoryConvert(static_cast<Category>(i)).size();
    if (length > maxLength) {
      maxLength = length;
    }
  }
  return maxLength;
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
                            const std::chrono::system_clock::time_point& tp) noexcept {
  using namespace std::chrono;

  auto seconds_epoch = floor<seconds>(tp);
  auto millis = duration_cast<milliseconds>(tp - seconds_epoch).count();

  std::time_t t = system_clock::to_time_t(tp);
  std::tm tm;
  localtime_r(&t, &tm);

  int len = std::snprintf(buffer.data(), buffer.size(), "%02d:%02d:%02d.%03lld", tm.tm_hour,
                          tm.tm_min, tm.tm_sec, (long long)millis);

  return std::string_view(buffer.data(), (len > 0 && (size_t)len < buffer.size()) ? len : 0);
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
  auto timeStr = FormatTime(mTimeBuffer, std::chrono::system_clock::now());
  auto levelStr = LevelConvert(level);
  auto categoryStr = CatergoryConvert(category);

  // take the guard right before printing to get correct log time at least
  std::lock_guard lock(mMutex);
  std::cout << "[" << timeStr << "][" << levelStr << "][" << std::setw(LongestCategoryLength())
            << std::left << std::setfill(' ') << categoryStr << "] " << msg << std::endl;
}
} // namespace Components::Logger
