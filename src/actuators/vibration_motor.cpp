#include <Arduino.h>
#include "actuators/vibration_motor.h"
#include "hardware/hardware_setup.h"

void vibrateWithIntensity(uint8_t intensity) {
  analogWrite(VIBRATION_MOTOR_PIN, intensity);
}

void stopVibration() {
  analogWrite(VIBRATION_MOTOR_PIN, 0);
}
