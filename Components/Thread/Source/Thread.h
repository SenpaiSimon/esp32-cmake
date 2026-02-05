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

  void Start(std::string_view name, ThreadPriority priority = ThreadPriority::Normal,
             uint8_t coreId = 0, uint32_t stackSize = CONFIG_PTHREAD_TASK_STACK_SIZE_DEFAULT) final;
  void Join() final;
  bool IsRunning() const final;

  virtual void Exec() = 0;

private:
  bool mRunning{false};
  std::thread mThread;

  esp_pthread_cfg_t CreateConfig(std::string_view name, ThreadPriority priority, uint8_t coreId,
                                 uint32_t stackSize);
};
} // namespace Components::Thread