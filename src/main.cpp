#include <Arduino.h>
#include "esp_sleep.h"

#define BLUE_LED_PIN 13
#define BUTTON_PIN 12

void enterDeepSleep() {
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, 0);
  Serial.println("[TERMINAL] Entrando em Deep Sleep");
  delay(100);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("[TERMINAL] Acordou do Deep Sleep");
    pinMode(BLUE_LED_PIN, OUTPUT);
    digitalWrite(BLUE_LED_PIN, HIGH);
  }
  else {
    Serial.println("[TERMINAL] Inicialização normal: Deep Sleep ativado");
    enterDeepSleep();
  }
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(150);
    if (digitalRead(BUTTON_PIN) == LOW) {
      enterDeepSleep();
    }
  }
}
