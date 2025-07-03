#include "tasks/button_task.h"
#include "hardware/hardware_setup.h"
#include "actuators/vibration_motor.h"
#include "power/power_manager.h"
#include <Arduino.h>

void buttonTask(void* parameter) {
  bool lastState = HIGH;

  while (true) {
    bool currentState = digitalRead(BUTTON_PIN);

    if (lastState == HIGH && currentState == LOW) {
      delay(50);  // debounce
      if (digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("[BUTTON] Pressed again, sleeping...");
        stopVibration();
        enterDeepSleep();
      }
    }

    lastState = currentState;
    vTaskDelay(pdMS_TO_TICKS(100));  // check every 100ms
  }
}
