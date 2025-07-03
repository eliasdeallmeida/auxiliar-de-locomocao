#include <Arduino.h>
#include "hardware/hardware_setup.h"

void configurePins() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT);
}

void turnOnLED() {
  digitalWrite(BLUE_LED_PIN, HIGH);
}
