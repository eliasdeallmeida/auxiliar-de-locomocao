#ifndef VIBRATION_MOTOR_H
#define VIBRATION_MOTOR_H

#include <Arduino.h>

void vibrateWithIntensity(uint8_t intensity); // 0 a 255
void stopVibration();

#endif
