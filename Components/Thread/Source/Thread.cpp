#include "Thread.h"

#include <utility>

namespace Components::Thread {
void Thread::Start(std::string_view name, ThreadPriority priority, uint8_t coreId,
                   uint32_t stackSize) {
  auto cfg = CreateConfig(name, priority, coreId, stackSize);
  esp_pthread_set_cfg(&cfg);

  mThread = std::thread([this]() { Exec(); });
  mRunning = true;
}

void Thread::Join() {
  if (!mRunning) {
    return;
  }

  mThread.join();
  mRunning = false;
}

bool Thread::IsRunning() const {
  return mRunning;
}

esp_pthread_cfg_t Thread::CreateConfig(std::string_view name, ThreadPriority priority,
                                       uint8_t coreId, uint32_t stackSize) {
  auto cfg = esp_pthread_get_default_config();
  cfg.thread_name = name.data();
  cfg.pin_to_core = coreId;
  cfg.stack_size = stackSize;
  cfg.prio = std::to_underlying(priority);
  return cfg;
}
} // namespace Components::Thread