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

  // Start the thread's execution.
  virtual void Start(std::string_view name, ThreadPriority priority, uint8_t coreId,
                     uint32_t stackSize) = 0;

  // Wait for the thread to finish execution.
  virtual void Join() = 0;

  // Check if the thread is currently running.
  virtual bool IsRunning() const = 0;
};
} // namespace Components::Thread
