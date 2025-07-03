#include <Arduino.h>
#include "hardware/hardware_setup.h"
#include "tasks/distance_task.h"
#include "tasks/button_task.h"
#include "power/power_manager.h"

void setup() {
  Serial.begin(115200);
  delay(100);

  configurePins();

  if (!isWakingFromDeepSleep()) {
    Serial.println("[BOOT] Normal startup. Going to sleep...");
    enterDeepSleep();
  }

  Serial.println("[BOOT] Woke up from Deep Sleep");
  turnOnLED();

  xTaskCreatePinnedToCore(distanceTask, "Distance Task", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(buttonTask, "Button Task", 2048, NULL, 2, NULL, 1);
}

void loop() {
  // empty – all logic is in tasks
}
