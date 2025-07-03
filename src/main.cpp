#include <Arduino.h>

#include "hardware_setup.h"
#include "power_manager.h"

void setup() {
  Serial.begin(115200);
  delay(100);

  configureButton();

  if (isWakingFromDeepSleep()) {
    Serial.println("[TERMINAL] Acordou do Deep Sleep");
    configurePowerLED();
    turnOnPowerLED();
  } else {
    Serial.println("[TERMINAL] Inicialização normal: Deep Sleep ativado");
    enterDeepSleep();
  }
}

void loop() {
  if (isButtonPressed()) {
    delay(150);
    enterDeepSleep();
  }
}
