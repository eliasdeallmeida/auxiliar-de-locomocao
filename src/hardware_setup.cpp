#include "hardware_setup.h"

void configurePowerLED() {
  pinMode(BLUE_LED_PIN, OUTPUT);
}

void turnOnPowerLED() {
  digitalWrite(BLUE_LED_PIN, HIGH);
}

void configureButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool isButtonPressed() {
  return digitalRead(BUTTON_PIN) == LOW;
}
