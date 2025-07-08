#include <Arduino.h>
#include "power/power_manager.h"
#include "hardware/hardware_setup.h"
#include "esp_sleep.h"
#include "logs/log_manager.h"

void enterDeepSleep() {
  esp_sleep_enable_ext0_wakeup((gpio_num_t)POWER_BUTTON_PIN, 0);
  logPrintln("[SLEEP] Entering Deep Sleep");
  delay(100);
  esp_deep_sleep_start();
}

bool isWakingFromDeepSleep() {
  return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0;
}
