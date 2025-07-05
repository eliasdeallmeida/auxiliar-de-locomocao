#include <Arduino.h>
#include "hardware/hardware_setup.h"

void configurePins() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BOT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT);
}

void turnOnPowerLED() {
  digitalWrite(GREEN_LED_PIN, HIGH);
}

void turnOnBotLED() {
  digitalWrite(BLUE_LED_PIN, HIGH);
}

void turnOffPowerLED() {
  digitalWrite(GREEN_LED_PIN, LOW);
}

void turnOffBotLED() {
  digitalWrite(BLUE_LED_PIN, LOW);
}
