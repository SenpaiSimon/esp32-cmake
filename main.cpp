#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <memory>
#include <stdio.h>

#include "Logger.h"

using Components::Logger::Category;
using Components::Logger::ILogger;
using Components::Logger::LogMessage;

extern "C" {
void app_main(void) {
  while (1) {
    std::unique_ptr<ILogger> logger = std::make_unique<Components::Logger::Logger>();
    logger->Info(Category::Init, "System initialized successfully.");
    logger->Warn(Category::Init, "Low memory warning.");
    logger->Error(Category::Init, "Failed to initialize component X.");
    logger->Trace(Category::Init, "Trace message.");

    logger->Info(Category::Init, LogMessage("Formatted number: %d", 42));

    try {
      throw std::runtime_error(LogMessage("Example exception %d", 123456789));
    } catch (const std::exception& e) {
      logger->Error(Category::Init, LogMessage("Caught exception: %s", e.what()));
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
}
