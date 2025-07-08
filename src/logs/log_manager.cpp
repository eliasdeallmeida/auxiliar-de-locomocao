#include "log_manager.h"
#include <LittleFS.h>
#include <time.h>

#define LOG_FILE "/logs.txt"
#define INDEX_FILE "/log_index.txt"

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
