#ifndef HARDWARE_SETUP_H
#define HARDWARE_SETUP_H

#define GREEN_LED_PIN 13
#define POWER_BUTTON_PIN 12
#define BLUE_LED_PIN 32
#define BOT_BUTTON_PIN 33
#define TRIGGER_PIN 27
#define ECHO_PIN 26
#define VIBRATION_MOTOR_PIN 25

void configurePins();
void turnOnPowerLED();
void turnOnBotLED();
void turnOffPowerLED();
void turnOffBotLED();

#endif
