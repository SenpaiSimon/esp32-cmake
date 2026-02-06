#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>
#include <stdio.h>

#include "Logger.h"
#include "Thread.h"

using Components::Logger::Category;
using Components::Logger::ILogger;
using Components::Logger::LogMessage;
using Components::Thread::IThread;
using Components::Thread::Thread;
using Components::Thread::ThreadPriority;

class TestThread : public Thread {
public:
  TestThread(ILogger& logger) : mLogger{logger}, Thread("TestThread", ThreadPriority::k5) {}
  ~TestThread() = default;

  void Exec() final {
    while (IsRunning()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      mLogger.Info(Category::Thread, LogMessage("%s is running", GetName().data()));
    }
  }

private:
  ILogger& mLogger;
};

extern "C" {
void app_main(void) {
  std::unique_ptr<ILogger> logger = std::make_unique<Components::Logger::Logger>();
  TestThread testThread(*logger);
  testThread.Start();

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    logger->Info(Category::Init, LogMessage("Main loop is alive"));
  }
}
}
