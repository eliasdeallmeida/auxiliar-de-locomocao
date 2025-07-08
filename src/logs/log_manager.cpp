#include "log_manager.h"
#include <LittleFS.h>
#include <time.h>
#include <stdarg.h>

#define LOG_FILE "/logs.txt"
#define INDEX_FILE "/log_index.txt"
#define SERIAL_LOG_FILE "/serial_log.txt"
#define MAX_SERIAL_LOG_SIZE 100000

String getLastLogs(int qtd) {
  File file = LittleFS.open("/logs.txt", "r");
  if (!file) return "";
  String content = file.readString();
  file.close();

  int count = 0;
  String output = "";
  for (int i = content.length() - 1; i >= 0; i--) {
    if (content[i] == '\n') count++;
    if (count == qtd) {
      output = content.substring(i + 1);
      break;
    }
  }
  if (output.length() == 0) return content;
  return output;
}

int getNextLogIndex() {
  File file = LittleFS.open(INDEX_FILE, FILE_READ);
  int index = 1;

  if (file) {
    index = file.readStringUntil('\n').toInt();
    file.close();
  }

  File writeFile = LittleFS.open(INDEX_FILE, FILE_WRITE);
  if (writeFile) {
    writeFile.println(index + 1);  // salva o próximo valor
    writeFile.close();
  }

  return index;
}

void appendLog(float distance, const String& description) {
  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timestamp[30];
  strftime(timestamp, sizeof(timestamp), "%d/%m/%Y %H:%M:%S", &timeinfo);

  int logNumber = getNextLogIndex();

  File file = LittleFS.open(LOG_FILE, FILE_APPEND);
  if (file) {
    file.printf("%d. %s - Distância: %.2f cm – %s\n",
                logNumber, timestamp, distance, description.c_str());
    file.close();
  } else {
    Serial.println("[LOG] Erro ao abrir arquivo de log.");
  }
}

bool clearLogs() {
  bool success = true;

  // Remove o arquivo de logs
  if (LittleFS.exists("/logs.txt")) {
    success &= LittleFS.remove("/logs.txt");
  }

  // Zera o contador de índice
  File indexFile = LittleFS.open("/log_index.txt", FILE_WRITE);
  if (indexFile) {
    indexFile.println(1);  // reinicia para 1
    indexFile.close();
  } else {
    success = false;
  }

  return success;
}

void logPrint(const String& msg) {
  Serial.print(msg);
  File file = LittleFS.open(SERIAL_LOG_FILE, FILE_APPEND);
  if (file) {
    file.print(msg);
    file.close();
    if (file.size() > MAX_SERIAL_LOG_SIZE) LittleFS.remove(SERIAL_LOG_FILE); // Limpa se passar do limite
  }
}

void logPrintln(const String& msg) {
  Serial.println(msg);
  File file = LittleFS.open(SERIAL_LOG_FILE, FILE_APPEND);
  if (file) {
    file.println(msg);
    file.close();
    if (file.size() > MAX_SERIAL_LOG_SIZE) LittleFS.remove(SERIAL_LOG_FILE);
  }
}

void logPrintf(const char* format, ...) {
  char buf[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  Serial.print(buf);
  File file = LittleFS.open(SERIAL_LOG_FILE, FILE_APPEND);
  if (file) {
    file.print(buf);
    file.close();
    if (file.size() > MAX_SERIAL_LOG_SIZE) LittleFS.remove(SERIAL_LOG_FILE);
  }
}
