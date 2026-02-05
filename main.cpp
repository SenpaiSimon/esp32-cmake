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

class ExampleThread : public Thread {
public:
  ExampleThread(ILogger& logger) : mLogger(logger), Thread("ExampleThread") {}

  void Exec() final {
    while (IsRunning()) {
      mLogger.Info(Category::Thread, LogMessage("Hello from %s!", GetName().data()));
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

private:
  ILogger& mLogger;
};

extern "C" {
void app_main(void) {
  std::unique_ptr<ILogger> logger = std::make_unique<Components::Logger::Logger>();
  ExampleThread thread(*logger);
  thread.Start();

  while (1) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    logger->Info(Category::Init, LogMessage("Main loop is alive"));
  }
}
}
