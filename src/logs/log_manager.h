#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <Arduino.h>

String getLastLogs(int count);
void appendLog(float distance, const String& description);
bool clearLogs();

#endif
