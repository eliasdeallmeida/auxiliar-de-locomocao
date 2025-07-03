#ifndef HARDWARE_SETUP_H
#define HARDWARE_SETUP_H

#include <Arduino.h>

#define BLUE_LED_PIN 13
#define BUTTON_PIN 12

void configurePowerLED();
void configureButton();
void turnOnPowerLED();
bool isButtonPressed();

#endif
