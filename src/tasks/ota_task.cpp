#include <Arduino.h>
#include "ota_task.h"
#include "../ota/ota_manager.h"
#include "../logs/log_manager.h"

void otaTask(void* parameter) {
  logPrintln("[OTA TASK] Iniciando task OTA");

  // Aguardar conexão WiFi
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  // Inicializar OTA
  initOTA();
  logPrintln("[OTA] Servidor OTA iniciado");

  while (true) {
    // Verificar se WiFi está conectado
    if (WiFi.status() == WL_CONNECTED) {
      // Processar requisições OTA
      handleOTA();

      // Verificar status do OTA
      OTAStatus status = getOTAStatus();
      if (status == OTA_SUCCESS) {
        logPrintln("[OTA] Atualização concluída com sucesso");
      } else if (status == OTA_ERROR) {
        OTAInfo info = getOTAInfo();
        logPrintln("[OTA] Erro na atualização: " + info.lastError);
      }
    } else {
      logPrintln("[OTA TASK] WiFi desconectado, aguardando reconexão...");
      logPrintln("[OTA] WiFi desconectado");
    }

    // Delay da task
    vTaskDelay(pdMS_TO_TICKS(100)); // 100ms
  }
}
