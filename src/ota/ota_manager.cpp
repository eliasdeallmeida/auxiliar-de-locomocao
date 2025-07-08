#include "ota_manager.h"
#include <Arduino.h>
#include <LittleFS.h>
#include "logs/log_manager.h"

// Variáveis globais do OTA
WebServer otaServer(OTA_PORT);
static OTAInfo otaInfo = {OTA_IDLE, 0, 0, 0, 0, ""};

// Páginas HTML para o OTA
const char* loginIndex = 
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
    "<meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<title>Auxiliar de Locomoção - OTA</title>"
    "<style>"
      "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f0f0f0; }"
      ".container { max-width: 600px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }"
      "h1 { color: #333; text-align: center; margin-bottom: 30px; }"
      "form { margin: 20px 0; }"
      "input[type='text'], input[type='password'] { width: 100%; padding: 12px; margin: 8px 0; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; }"
      "input[type='submit'] { background-color: #4CAF50; color: white; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer; width: 100%; }"
      "input[type='submit']:hover { background-color: #45a049; }"
      ".error { color: red; text-align: center; margin: 10px 0; }"
    "</style>"
  "</head>"
  "<body>"
    "<div class='container'>"
      "<h1>🔧 Atualização OTA</h1>"
      "<form name='loginForm'>"
        "<div>"
          "<label for='userid'>Usuário:</label>"
          "<input type='text' id='userid' name='userid' required>"
        "</div>"
        "<div>"
          "<label for='pwd'>Senha:</label>"
          "<input type='password' id='pwd' name='pwd' required>"
        "</div>"
        "<input type='submit' value='Entrar' onclick='check(this.form)'>"
      "</form>"
      "<div id='error' class='error' style='display:none;'></div>"
    "</div>"
    "<script>"
      "function check(form) {"
        "event.preventDefault();"
        "var userid = form.userid.value;"
        "var pwd = form.pwd.value;"
        "if(userid == '" OTA_USERNAME "' && pwd == '" OTA_PASSWORD "') {"
          "window.location.href = '/update';"
        "} else {"
          "document.getElementById('error').innerHTML = 'Usuário ou senha inválidos';"
          "document.getElementById('error').style.display = 'block';"
        "}"
      "}"
    "</script>"
  "</body>"
  "</html>";

const char* serverIndex = 
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
    "<meta charset='utf-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>"
    "<title>OTA e Logs - Auxiliar de Locomoção</title>"
    "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<style>"
      "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f0f0f0; }"
      ".container { max-width: 600px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }"
      "h1 { color: #333; text-align: center; margin-bottom: 30px; }"
      ".tabs { display: flex; border-bottom: 1px solid #ddd; margin-bottom: 20px; }"
      ".tab { flex: 1; text-align: center; padding: 12px; cursor: pointer; background: #f9f9f9; border: none; outline: none; font-size: 16px; transition: background 0.2s; }"
      ".tab.active { background: #fff; border-bottom: 2px solid #2196F3; color: #2196F3; font-weight: bold; }"
      ".tab-content { display: none; }"
      ".tab-content.active { display: block; }"
      ".upload-form { margin: 20px 0; }"
      "input[type='file'] { width: 100%; padding: 12px; margin: 8px 0; border: 1px solid #ddd; border-radius: 4px; box-sizing: border-box; }"
      "input[type='submit'] { background-color: #2196F3; color: white; padding: 12px 20px; border: none; border-radius: 4px; cursor: pointer; width: 100%; margin-top: 10px; }"
      "input[type='submit']:hover { background-color: #1976D2; }"
      ".progress { margin: 20px 0; }"
      ".progress-bar { width: 100%; background-color: #f0f0f0; border-radius: 4px; overflow: hidden; }"
      ".progress-fill { height: 20px; background-color: #4CAF50; width: 0%; transition: width 0.3s; }"
      ".status { text-align: center; margin: 10px 0; font-weight: bold; }"
      ".success { color: #4CAF50; }"
      ".error { color: #f44336; }"
      ".info { color: #2196F3; }"
      ".logs-area { width: 100%; height: 350px; background: #222; color: #0f0; font-family: monospace; font-size: 14px; border-radius: 6px; padding: 12px; overflow-y: auto; white-space: pre-wrap; box-sizing: border-box; }"
    "</style>"
  "</head>"
  "<body>"
    "<div class='container'>"
      "<h1>Auxiliar de Locomoção</h1>"
      "<div class='tabs'>"
        "<button class='tab active' onclick='showTab(0)'>Atualização OTA</button>"
        "<button class='tab' onclick='showTab(1)'>Logs do Terminal</button>"
      "</div>"
      "<div class='tab-content active' id='tab-ota'>"
        "<h2>📤 Atualização de Firmware</h2>"
        "<div class='upload-form'>"
          "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
            "<input type='file' name='update' accept='.bin' required>"
            "<input type='submit' value='Atualizar Firmware'>"
          "</form>"
        "</div>"
        "<div class='progress'>"
          "<div class='progress-bar'>"
            "<div class='progress-fill' id='progress-fill'></div>"
          "</div>"
          "<div class='status' id='status'>Aguardando arquivo...</div>"
        "</div>"
      "</div>"
      "<div class='tab-content' id='tab-logs'>"
        "<h2>📝 Logs do Terminal</h2>"
        "<div class='logs-area' id='logs-area'>Carregando logs...</div>"
      "</div>"
    "</div>"
    "<script>"
      "function showTab(idx) {"
        "$('.tab').removeClass('active');"
        "$('.tab-content').removeClass('active');"
        "$('.tab').eq(idx).addClass('active');"
        "$('.tab-content').eq(idx).addClass('active');"
      "}"
      "$('form').submit(function(e) {"
        "e.preventDefault();"
        "var form = $('#upload_form')[0];"
        "var data = new FormData(form);"
        "$('#status').html('Iniciando upload...').removeClass('success error').addClass('info');"
        "$('#progress-fill').css('width', '0%');"
        "$.ajax({"
          "url: '/doUpdate',"
          "type: 'POST',"
          "data: data,"
          "contentType: false,"
          "processData: false,"
          "xhr: function() {"
            "var xhr = new window.XMLHttpRequest();"
            "xhr.upload.addEventListener('progress', function(evt) {"
              "if (evt.lengthComputable) {"
                "var per = evt.loaded / evt.total;"
                "$('#progress-fill').css('width', Math.round(per*100) + '%');"
                "$('#status').html('Progresso: ' + Math.round(per*100) + '%').removeClass('success error').addClass('info');"
              "}"
            "}, false);"
            "return xhr;"
          "},"
          "success: function(d, s) {"
            "$('#status').html('✅ Atualização concluída! Reiniciando...').removeClass('error info').addClass('success');"
            "setTimeout(function() { window.location.reload(); }, 3000);"
          "},"
          "error: function (a, b, c) {"
            "$('#status').html('❌ Erro na atualização: ' + c).removeClass('success info').addClass('error');"
          "}"
        "});"
      "});"
      "function fetchLogs() {"
        "$.ajax({"
          "url: '/logs',"
          "type: 'GET',"
          "success: function(data) {"
            "$('#logs-area').text(data);"
            "var logsArea = document.getElementById('logs-area');"
            "logsArea.scrollTop = logsArea.scrollHeight;"
          "},"
          "error: function() {"
            "$('#logs-area').text('Erro ao carregar logs.');"
          "}"
        "});"
      "}"
      "setInterval(fetchLogs, 2000);"
      "fetchLogs();"
    "</script>"
  "</body>"
  "</html>";

void initOTA() {
  // Configurar mDNS
  if (!MDNS.begin(OTA_HOST)) {
    Serial.println("[OTA] Erro ao configurar mDNS");
    return;
  }
  
  Serial.println("[OTA] mDNS configurado: " + String(OTA_HOST) + ".local");

  // Configurar rotas do servidor
  otaServer.on("/", HTTP_GET, handleRoot);
  otaServer.on("/update", HTTP_GET, handleServerIndex);
  otaServer.on("/doUpdate", HTTP_POST, handleUpdate, handleUpdateProgress);
  otaServer.on("/logs", HTTP_GET, handleLogs); // <-- Adiciona rota de logs
  
  // Iniciar servidor
  otaServer.begin();
  Serial.println("[OTA] Servidor OTA iniciado na porta " + String(OTA_PORT));
  Serial.println("[OTA] Acesse: http://" + String(OTA_HOST) + ".local ou http://" + WiFi.localIP().toString());
}

void handleOTA() {
  otaServer.handleClient();
//   MDNS.update();
}

OTAStatus getOTAStatus() {
  return otaInfo.status;
}

OTAInfo getOTAInfo() {
  return otaInfo;
}

void resetOTAStatus() {
  otaInfo.status = OTA_IDLE;
  otaInfo.lastError = "";
  otaInfo.updateProgress = 0;
  otaInfo.updateTotalSize = 0;
}

bool isOTAEnabled() {
  return WiFi.status() == WL_CONNECTED;
}

void handleRoot() {
  otaServer.sendHeader("Connection", "close");
  otaServer.send(200, "text/html", loginIndex);
}

void handleServerIndex() {
  otaServer.sendHeader("Connection", "close");
  otaServer.send(200, "text/html", serverIndex);
}

void handleUpdate() {
  otaServer.sendHeader("Connection", "close");
  otaServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  
  if (!Update.hasError()) {
    Serial.println("[OTA] Atualização concluída com sucesso");
    otaInfo.status = OTA_SUCCESS;
    otaInfo.lastUpdateTime = millis();
    
    // Reiniciar após 2 segundos
    delay(2000);
    ESP.restart();
  } else {
    Serial.println("[OTA] Erro na atualização");
    otaInfo.status = OTA_ERROR;
    otaInfo.lastError = "Erro durante a atualização";
  }
}

void handleUpdateProgress() {
  HTTPUpload& upload = otaServer.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("[OTA] Iniciando atualização: %s\n", upload.filename.c_str());
    otaInfo.status = OTA_UPDATING;
    otaInfo.updateStartTime = millis();
    otaInfo.updateProgress = 0;
    otaInfo.updateTotalSize = 0;
    
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
      Update.printError(Serial);
      otaInfo.status = OTA_ERROR;
      otaInfo.lastError = "Falha ao iniciar atualização";
    }
  }
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
      otaInfo.status = OTA_ERROR;
      otaInfo.lastError = "Erro ao escrever dados";
    } else {
      otaInfo.updateProgress += upload.currentSize;
      otaInfo.updateTotalSize = upload.totalSize;
      
      // Log do progresso a cada 10%
      if (otaInfo.updateTotalSize > 0) {
        int progress = (otaInfo.updateProgress * 100) / otaInfo.updateTotalSize;
        if (progress % 10 == 0) {
          Serial.printf("[OTA] Progresso: %d%%\n", progress);
        }
      }
    }
  }
  else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      Serial.printf("[OTA] Atualização concluída: %u bytes\n", upload.totalSize);
      otaInfo.updateProgress = upload.totalSize;
      otaInfo.updateTotalSize = upload.totalSize;
    } else {
      Update.printError(Serial);
      otaInfo.status = OTA_ERROR;
      otaInfo.lastError = "Falha ao finalizar atualização";
    }
  }
}

void handleLogs() {
  File file = LittleFS.open("/serial_log.txt", "r");
  if (!file) {
    otaServer.send(200, "text/plain", "(sem logs)");
    return;
  }
  String logs = file.readString();
  file.close();
  otaServer.sendHeader("Access-Control-Allow-Origin", "*");
  otaServer.send(200, "text/plain", logs);
}
