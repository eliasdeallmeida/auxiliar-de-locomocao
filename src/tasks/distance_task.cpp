#include "tasks/distance_task.h"
#include "sensors/distance_sensor.h"
#include "actuators/vibration_motor.h"
#include "logs/log_manager.h"
#include "config/distance_config.h"
#include <Arduino.h>
#include "tasks/mp3_task.h"

void distanceTask(void* parameter) {
  while (true) {
    float distance = readMedianDistance();
    logPrintf("[DISTANCE] %.2f cm\n", distance);
    requestProximityAudio(distance);

    // Obtém os limites configurados
    DistanceLimits limits = getDistanceLimits();
    
    String message;
    
    if (distance <= limits.proximo) {
      message = "Próximo";
      logPrintln(">> " + message);
      vibrateWithIntensity(255);
    }
    else if (distance <= limits.aproximando) {
      message = "Aproximando";
      logPrintln(">> " + message);
      vibrateWithIntensity(190);
    }
    else if (distance <= limits.distante) {
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
