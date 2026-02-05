#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>
#include <stdio.h>

#include "Logger.h"
#include "Thread.h"

using Components::Logger::Category;
using Components::Logger::ILogger;
using Components::Logger::LogMessage;

class ExampleThread : public Components::Thread::Thread {
public:
  ExampleThread(std::string_view name, ILogger& logger) : mName(name), mLogger(logger) {
  }

  void Exec() final {
    while (IsRunning()) {
      mLogger.Info(Category::Init, LogMessage("Hello from %s!", mName.c_str()));
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }

private:
  std::string mName;
  ILogger& mLogger;
};

extern "C" {
void app_main(void) {
  std::unique_ptr<ILogger> logger = std::make_unique<Components::Logger::Logger>();
  ExampleThread thread("TestTestPass", *logger);
  thread.Start("ExampleThread");

  while (1) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    logger->Info(Category::Init, LogMessage("Main loop is alive"));
  }
}
}
