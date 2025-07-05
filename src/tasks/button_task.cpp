#include "tasks/button_task.h"
#include "hardware/hardware_setup.h"
#include "actuators/vibration_motor.h"
#include "power/power_manager.h"
#include <Arduino.h>

void buttonTask(void* parameter) {
  bool lastState = HIGH;

  while (true) {
    bool currentState = digitalRead(POWER_BUTTON_PIN);

    if (lastState == HIGH && currentState == LOW) {
      delay(50);
      if (digitalRead(POWER_BUTTON_PIN) == LOW) {
        Serial.println("[BOTÃO] Pressionado, entrando em Deep Sleep...");
        stopVibration();
        while (digitalRead(POWER_BUTTON_PIN) == LOW) {
          vTaskDelay(pdMS_TO_TICKS(50));  // Espera o botão ser solto
        }
        enterDeepSleep();
      }
    }

    lastState = currentState;
    vTaskDelay(pdMS_TO_TICKS(100));  // check every 100ms
  }
}
