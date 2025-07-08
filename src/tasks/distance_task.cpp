#include "tasks/distance_task.h"
#include "sensors/distance_sensor.h"
#include "actuators/vibration_motor.h"
#include "logs/log_manager.h"
#include <Arduino.h>
#include "tasks/mp3_task.h"

void distanceTask(void* parameter) {
  while (true) {
    float distance = readMedianDistance();
    logPrintf("[DISTANCE] %.2f cm\n", distance);
    requestProximityAudio(distance);

    String message;
    
    if (distance <= 10.0) {
      message = "Próximo";
      logPrintln(">> " + message);
      vibrateWithIntensity(255);
    }
    else if (distance <= 20.0) {
      message = "Aproximando";
      logPrintln(">> " + message);
      vibrateWithIntensity(190);
    }
    else if (distance <= 30.0) {
      message = "Distante";
      logPrintln(">> " + message);
      vibrateWithIntensity(130);
    }
    else {
      message = "Fora de alcance";
      logPrintln(">> " + message);
    }
    appendLog(distance, message);
    delay(1000);
    stopVibration();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
