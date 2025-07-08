#include "mp3_task.h"
#include "actuators/mp3_player.h"
#include <Arduino.h>

static QueueHandle_t mp3Queue;

void requestProximityAudio(float distance) {
    if (mp3Queue) {
        float d = distance;
        xQueueSend(mp3Queue, &d, 0);
    }
}

void mp3Task(void* parameter) {
    mp3Queue = xQueueCreate(5, sizeof(float));
    initMP3Player();
    while (true) {
        float distance = 0;
        if (xQueueReceive(mp3Queue, &distance, portMAX_DELAY) == pdTRUE) {
            int nivel = 0;
            VoiceGender gender = getVoiceGender();
            if (gender == MALE) {
                if (distance <= 10.0) nivel = 1;
                else if (distance <= 20.0) nivel = 2;
                else if (distance <= 30.0) nivel = 3;
            } else {
                if (distance <= 10.0) nivel = 4;
                else if (distance <= 20.0) nivel = 5;
                else if (distance <= 30.0) nivel = 6;
            }
            if (nivel > 0) {
                playProximityAudio(nivel);
            }
        }
    }
} 