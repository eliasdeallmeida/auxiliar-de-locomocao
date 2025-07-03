#include <Arduino.h>
#include "sensors/distance_sensor.h"
#include "hardware/hardware_setup.h"

float measureDistanceCM() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2.0;
}

float readMedianDistance() {
  const int count = 101;
  float values[count];

  for (int i = 0; i < count; i++) {
    values[i] = measureDistanceCM();
    delay(5);
  }

  std::sort(values, values + count);
  return values[count / 2];
}
