#include "power_manager.h"
#include "hardware_setup.h"
#include "esp_sleep.h"

void enterDeepSleep() {
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, 0);
  Serial.println("[TERMINAL] Entrou em Deep Sleep");
  delay(100);
  esp_deep_sleep_start();
}

bool isWakingFromDeepSleep() {
  return esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0;
}
