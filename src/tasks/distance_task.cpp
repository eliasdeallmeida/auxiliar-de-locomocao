#include "tasks/distance_task.h"
#include "sensors/distance_sensor.h"
#include "actuators/vibration_motor.h"
#include <Arduino.h>

void distanceTask(void* parameter) {
  while (true) {
    float distance = readMedianDistance();
    Serial.printf("[DISTANCE] %.2f cm\n", distance);

    if (distance <= 10.0) {
      Serial.println(">> Close");
      vibrateWithIntensity(255);
    } else if (distance <= 20.0) {
      Serial.println(">> Ahead");
      vibrateWithIntensity(180);
    } else if (distance <= 30.0) {
      Serial.println(">> Approaching");
      vibrateWithIntensity(120);
    } else {
      Serial.println(">> Out of range");
    }
    delay(1000);
    stopVibration();
    vTaskDelay(pdMS_TO_TICKS(2000));  // wait 5 seconds
  }
}
