#include "Thread.h"

#include <utility>

namespace Components::Thread {
Thread::Thread(std::string_view name, ThreadPriority priority, uint8_t coreId, uint32_t stackSize)
    : mName{name}, mPriority{priority}, mCoreId{coreId}, mStackSize{stackSize} {}

void Thread::Start() {
  auto cfg = CreateConfig(mName, mPriority, mCoreId, mStackSize);
  esp_pthread_set_cfg(&cfg);

  mRunning = true;
  mThread = std::thread([this]() { Exec(); });
}

void Thread::Stop() {
  if (!mRunning) {
    throw std::runtime_error("Thread is not running");
  }

  mRunning = false;
  mThread.join();
}

bool Thread::IsRunning() const noexcept { return mRunning; }

std::string_view Thread::GetName() const noexcept { return mName; }

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