#pragma once

#include <string_view>

namespace Components::Thread {
enum class ThreadPriority : int {
  Low = 1,
  Normal = CONFIG_PTHREAD_TASK_PRIO_DEFAULT,
  High = 2 * CONFIG_PTHREAD_TASK_PRIO_DEFAULT
};

class IThread {
public:
  virtual ~IThread() = default;

  // Wait for the thread to finish execution.
  virtual void Stop() = 0;

  // Check if the thread is currently running.
  virtual bool IsRunning() const noexcept = 0;

  // Get your name
  virtual std::string_view GetName() const noexcept = 0;
};
} // namespace Components::Thread
