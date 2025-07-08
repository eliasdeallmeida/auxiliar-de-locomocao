#ifndef OTA_TASK_H
#define OTA_TASK_H

#include <Arduino.h>

// Task OTA
void otaTask(void* parameter);

// Configurações da task
#define OTA_TASK_STACK_SIZE 8192
#define OTA_TASK_PRIORITY 1
#define OTA_TASK_CORE 1

#endif // OTA_TASK_H
