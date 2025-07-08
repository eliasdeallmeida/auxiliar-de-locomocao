#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <Arduino.h>

String getLastLogs(int count);
void appendLog(float distance, const String& description);
bool clearLogs();
void logPrint(const String& msg);
void logPrintln(const String& msg);
void logPrintf(const char* format, ...);

#endif
