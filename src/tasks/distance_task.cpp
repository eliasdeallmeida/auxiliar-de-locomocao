#include "tasks/distance_task.h"
#include "sensors/distance_sensor.h"
#include "actuators/vibration_motor.h"
#include "logs/log_manager.h"
#include <Arduino.h>

void distanceTask(void* parameter) {
  while (true) {
    float distance = readMedianDistance();
    Serial.printf("[DISTANCE] %.2f cm\n", distance);

    String message;
    
    if (distance <= 10.0) {
      message = "Próximo";
      Serial.println(">> " + message);
      vibrateWithIntensity(255);
    }
    else if (distance <= 20.0) {
      message = "Aproximando";
      Serial.println(">> " + message);
      vibrateWithIntensity(190);
    }
    else if (distance <= 30.0) {
      message = "Distante";
      Serial.println(">> " + message);
      vibrateWithIntensity(130);
    }
    else {
      message = "Fora de alcance";
      Serial.println(">> " + message);
    }
    appendLog(distance, message);
    delay(1000);
    stopVibration();
    vTaskDelay(pdMS_TO_TICKS(2000));  // wait 5 seconds
  }
}
