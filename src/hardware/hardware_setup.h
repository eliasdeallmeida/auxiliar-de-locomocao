#ifndef HARDWARE_SETUP_H
#define HARDWARE_SETUP_H

#define BLUE_LED_PIN 13
#define BUTTON_PIN 12
#define TRIGGER_PIN 27
#define ECHO_PIN 26
#define VIBRATION_MOTOR_PIN 25

void configurePins();
void turnOnLED();
bool isExitButtonPressed();

#endif
