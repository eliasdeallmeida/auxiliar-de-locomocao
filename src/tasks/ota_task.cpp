#include <Arduino.h>
#include "ota_task.h"
#include "../ota/ota_manager.h"
#include "../logs/log_manager.h"

void otaTask(void* parameter) {
  Serial.println("[OTA TASK] Iniciando task OTA");

  // Aguardar conexão WiFi
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  // Inicializar OTA
  initOTA();
  Serial.println("[OTA] Servidor OTA iniciado");

  while (true) {
    // Verificar se WiFi está conectado
    if (WiFi.status() == WL_CONNECTED) {
      // Processar requisições OTA
      handleOTA();

      // Verificar status do OTA
      OTAStatus status = getOTAStatus();
      if (status == OTA_SUCCESS) {
        Serial.println("[OTA] Atualização concluída com sucesso");
      } else if (status == OTA_ERROR) {
        OTAInfo info = getOTAInfo();
        Serial.println("[OTA] Erro na atualização: " + info.lastError);
      }
    } else {
      Serial.println("[OTA TASK] WiFi desconectado, aguardando reconexão...");
      Serial.println("[OTA] WiFi desconectado");
    }

    // Delay da task
    vTaskDelay(pdMS_TO_TICKS(100)); // 100ms
  }
}
