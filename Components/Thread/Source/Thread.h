#pragma once

#include "IThread.h"
#include <esp_pthread.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <thread>

namespace Components::Thread {
class Thread : public IThread {
public:
  ~Thread() = default;

  Thread(std::string_view name, ThreadPriority priority = ThreadPriority::k5, uint8_t coreId = 0,
         uint32_t stackSize = CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT);

  // delete copy and move constructors and assignment operators
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;
  Thread(Thread&&) = delete;
  Thread& operator=(Thread&&) = delete;

  void Start();
  void Stop() final;
  bool IsRunning() const noexcept final;
  std::string_view GetName() const noexcept final;

  virtual void Exec() = 0;

private:
  bool mRunning{false};
  std::thread mThread;
  std::string mName;
  ThreadPriority mPriority{};
  uint8_t mCoreId{};
  uint32_t mStackSize{};

  esp_pthread_cfg_t CreateConfig(std::string_view name, ThreadPriority priority, uint8_t coreId,
                                 uint32_t stackSize);
};
} // namespace Components::Thread