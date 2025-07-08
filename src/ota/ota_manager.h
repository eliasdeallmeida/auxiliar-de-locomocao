#ifndef OTA_MANAGER_H
#define OTA_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

// Configurações OTA
#define OTA_HOST "auxiliar-locomocao"
#define OTA_PORT 80
#define OTA_USERNAME "admin"
#define OTA_PASSWORD "admin123"

// Estados do OTA
enum OTAStatus {
  OTA_IDLE,
  OTA_UPDATING,
  OTA_SUCCESS,
  OTA_ERROR
};

// Estrutura para informações do OTA
struct OTAInfo {
  OTAStatus status;
  unsigned long lastUpdateTime;
  unsigned long updateStartTime;
  size_t updateProgress;
  size_t updateTotalSize;
  String lastError;
};

// Funções do OTA Manager
void initOTA();
void handleOTA();
OTAStatus getOTAStatus();
OTAInfo getOTAInfo();
void resetOTAStatus();
bool isOTAEnabled();

// Funções internas
void handleRoot();
void handleServerIndex();
void handleUpdate();
void handleUpdateProgress();
void handleLogs();

#endif // OTA_MANAGER_H
