#include "log_manager.h"
#include <LittleFS.h>
#include <time.h>

#define LOG_FILE "/logs.txt"
#define INDEX_FILE "/log_index.txt"
#define MAX_LINES 100

String getLastLogs(int count) {
  File file = LittleFS.open(LOG_FILE, FILE_READ);
  if (!file) return "Nenhum log encontrado.";

  String lines[MAX_LINES];
  int total = 0;

  while (file.available() && total < MAX_LINES) {
    lines[total++] = file.readStringUntil('\n');
  }
  file.close();

  int start = max(0, total - count);
  int realCount = total - start;

  String result = "📄 Últimos " + String(realCount) + " logs:\n";
  for (int i = start; i < total; i++) {
    result += lines[i] + "\n";
  }

  return result;
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
