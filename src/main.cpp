#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <LittleFS.h>

#include "hardware/hardware_setup.h"
#include "logs/log_manager.h"
#include "power/power_manager.h"
#include "tasks/distance_task.h"
#include "tasks/button_task.h"
#include "tasks/mp3_task.h"
#include "tasks/ota_task.h"
#include "actuators/mp3_player.h"
#include "actuators/vibration_motor.h"
#include "ota/ota_manager.h"
#include "config/distance_config.h"

#define BOT_TOKEN "7258679771:AAGV6i7Lr5UAhMstclHGn0ETOYbd2vGZLvo"
#define CHAT_ID "976525165"

#define SSID "iPhone de Isa"
#define PASSWORD "isa12345"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

time_t lastTimeReading = 0;

unsigned long lastTimeBotRan;
int botRequestDelay = 1000;

bool awaitingLogCount = false;

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String from_name = bot.messages[i].from_name;

    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Usuário não autorizado", "");
      continue;
    }

    String text = bot.messages[i].text;
    logPrintln("Comando recebido: " + text);

    if (awaitingLogCount) {
      int count = text.toInt();
      if (count <= 0) count = 20;

      String logs = getLastLogs(count);
      bot.sendMessage(chat_id, logs, "");
      awaitingLogCount = false;
    }

    else if (text == "/start") {
      String welcome = "Bem vindo, " + from_name + "!\n";
      welcome += "Use os comandos:\n";
      welcome += "/logs\n";
      welcome += "/clearlogs\n";
      welcome += "/voz\n";
      welcome += "/volume <0-30>\n";
      welcome += "/alterardistancia <próximo> <aproximando> <distante>\n";
      welcome += "/distancia\n";
      welcome += "/ota\n";
      welcome += "/otastatus";
      bot.sendMessage(chat_id, welcome, "");
    }

    else if (text == "/logs") {
      String logs = getLastLogs(20);
      bot.sendMessage(chat_id, logs, "");
    }
    else if (text == "/clearlogs") {
      bool success = clearLogs();
      if (success) {
        bot.sendMessage(chat_id, "✅ Todos os logs foram apagados e o contador foi reiniciado.", "");
      } else {
        bot.sendMessage(chat_id, "❌ Erro ao apagar os logs.", "");
      }
    }
    else if (text == "/voz") {
      VoiceGender gender = getVoiceGender();
      if (gender == MALE) {
        setVoiceGender(FEMALE);
        bot.sendMessage(chat_id, "🔄 Voz alterada para feminina.", "");
      } else {
        setVoiceGender(MALE);
        bot.sendMessage(chat_id, "🔄 Voz alterada para masculina.", "");
      }
    }
    else if (text.startsWith("/volume")) {
      // Verifica se o comando tem parâmetros
      if (text == "/volume") {
        String helpMsg = "🔊 **Comando Volume**\n\n";
        helpMsg += "📋 **Uso correto:**\n";
        helpMsg += "`/volume <valor>`\n\n";
        helpMsg += "📊 **Parâmetros:**\n";
        helpMsg += "• `<valor>`: Número entre 0 e 30\n\n";
        helpMsg += "🔊 **Volume atual:** " + String(getCurrentVolume()) + "/30";
        bot.sendMessage(chat_id, helpMsg, "Markdown");
      } else {
        // Extrai o valor do volume do comando
        String volumeStr = text.substring(8); // Remove "/volume "
        volumeStr.trim(); // Remove espaços em branco
        
        if (volumeStr.length() == 0) {
          String helpMsg = "🔊 **Comando Volume**\n\n";
          helpMsg += "📋 **Uso correto:**\n";
          helpMsg += "`/volume <valor>`\n\n";
          helpMsg += "📊 **Parâmetros:**\n";
          helpMsg += "• `<valor>`: Número entre 0 e 30\n\n";
          helpMsg += "🔊 **Volume atual:** " + String(getCurrentVolume()) + "/30";
          bot.sendMessage(chat_id, helpMsg, "Markdown");
        } else {
          int volume = volumeStr.toInt();
          
          if (volume >= 0 && volume <= 30) {
            setVolume(volume);
            bot.sendMessage(chat_id, "🔊 Volume alterado para: " + String(volume), "");
          } else {
            bot.sendMessage(chat_id, "❌ Volume deve estar entre 0 e 30. Exemplo: /volume 15", "");
          }
        }
      }
    }
    else if (text.startsWith("/alterardistancia")) {
      // Extrai os valores de distância do comando
      String params = text.substring(17); // Remove "/alterardistancia "
      params.trim();
      
      // Divide os parâmetros por espaço
      int space1 = params.indexOf(' ');
      int space2 = params.indexOf(' ', space1 + 1);
      
      if (space1 > 0 && space2 > space1) {
        float proximo = params.substring(0, space1).toFloat();
        float aproximando = params.substring(space1 + 1, space2).toFloat();
        float distante = params.substring(space2 + 1).toFloat();
        
        if (validateDistanceLimits(proximo, aproximando, distante)) {
          setDistanceLimits(proximo, aproximando, distante);
          String message = "📏 **Limites de distância atualizados:**\n\n";
          message += "🟢 **Próximo:** " + String(proximo, 1) + " cm\n";
          message += "🟡 **Aproximando:** " + String(aproximando, 1) + " cm\n";
          message += "🔴 **Distante:** " + String(distante, 1) + " cm\n\n";
          message += "✅ Configuração salva com sucesso!";
          bot.sendMessage(chat_id, message, "Markdown");
        } else {
          String errorMsg = "❌ **Valores inválidos!**\n\n";
          errorMsg += "📋 **Regras:**\n";
          errorMsg += "• Valores devem estar entre 1 e 500 cm\n";
          errorMsg += "• Próximo < Aproximando < Distante\n";
          errorMsg += "• Use números decimais (ex: 10.5)\n\n";
          errorMsg += "💡 **Exemplo:** `/alterardistancia 8.5 15.0 25.0`";
          bot.sendMessage(chat_id, errorMsg, "Markdown");
        }
      } else {
        String errorMsg = "❌ **Formato inválido!**\n\n";
        errorMsg += "📋 **Uso correto:**\n";
        errorMsg += "`/alterardistancia <próximo> <aproximando> <distante>`\n\n";
        errorMsg += "💡 **Exemplo:** `/alterardistancia 8.5 15.0 25.0`\n\n";
        errorMsg += "📊 **Valores atuais:**\n";
        DistanceLimits current = getDistanceLimits();
        errorMsg += "• Próximo: " + String(current.proximo, 1) + " cm\n";
        errorMsg += "• Aproximando: " + String(current.aproximando, 1) + " cm\n";
        errorMsg += "• Distante: " + String(current.distante, 1) + " cm";
        bot.sendMessage(chat_id, errorMsg, "Markdown");
             }
     }
     else if (text == "/distancia") {
       DistanceLimits limits = getDistanceLimits();
       String message = "📏 **Limites de distância atuais:**\n\n";
       message += "🟢 **Próximo:** " + String(limits.proximo, 1) + " cm\n";
       message += "🟡 **Aproximando:** " + String(limits.aproximando, 1) + " cm\n";
       message += "🔴 **Distante:** " + String(limits.distante, 1) + " cm\n\n";
       message += "💡 Use `/alterardistancia` para modificar estes valores.";
       bot.sendMessage(chat_id, message, "Markdown");
     }
     else if (text == "/ota") {
      if (isOTAEnabled()) {
        String ip = WiFi.localIP().toString();
        String message = "🔧 **Servidor OTA Ativo**\n\n";
        message += "📍 **IP:** " + ip + "\n";
        message += "🌐 **URL:** http://" + ip + "\n";
        message += "🔑 **Usuário:** admin\n";
        message += "🔐 **Senha:** admin123\n\n";
        message += "📱 Acesse o link acima para atualizar o firmware.";
        bot.sendMessage(chat_id, message, "Markdown");
      } else {
        bot.sendMessage(chat_id, "❌ OTA não disponível - WiFi desconectado", "");
      }
    }
    else if (text == "/otastatus") {
      OTAStatus status = getOTAStatus();
      OTAInfo info = getOTAInfo();
      
      String message = "🔧 **Status OTA**\n\n";
      
      switch (status) {
        case OTA_IDLE:
          message += "📊 **Status:** Ocioso\n";
          break;
        case OTA_UPDATING:
          message += "📤 **Status:** Atualizando...\n";
          if (info.updateTotalSize > 0) {
            int progress = (info.updateProgress * 100) / info.updateTotalSize;
            message += "📈 **Progresso:** " + String(progress) + "%\n";
          }
          break;
        case OTA_SUCCESS:
          message += "✅ **Status:** Atualização concluída\n";
          break;
        case OTA_ERROR:
          message += "❌ **Status:** Erro na atualização\n";
          message += "💬 **Erro:** " + info.lastError + "\n";
          break;
      }
      
      if (info.lastUpdateTime > 0) {
        message += "🕒 **Última atualização:** " + String(info.lastUpdateTime) + "ms atrás\n";
      }
      
      bot.sendMessage(chat_id, message, "Markdown");
    }
  }
}

void processTelegram() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void telegramTask(void* parameter) {
  while (true) {
    processTelegram();
    vTaskDelay(pdMS_TO_TICKS(1000));  // Executa a cada segundo
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  configurePins();

  if (!LittleFS.begin(true)) {
    logPrintln("[ERRO] LittleFS não pôde ser montado.");
  }

  // Inicializar configuração de distância
  initDistanceConfig();

  if (!isWakingFromDeepSleep()) {
    logPrintln("[BOOT] Normal startup. Going to sleep...");
    enterDeepSleep();
  }

  logPrintln("[BOOT] Woke up from Deep Sleep");
  turnOnPowerLED();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif

  // Criar tasks primeiro para inicializar o MP3 player

  xTaskCreatePinnedToCore(mp3Task, "MP3 Task", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(buttonTask, "Button Task", 2048, NULL, 2, NULL, 1);


  // Aguardar um pouco para o MP3 player inicializar
  delay(1000);

  // Tocar áudio de conexão WiFi e iniciar vibração
  playWifiConnectionAudio();
  vibrateWithIntensity(128); // Vibração média durante conexão

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    logPrintln("Conectando com o WiFi...");
  }

  // Parar vibração quando conectar
  stopVibration();

  logPrintln("IP adquirido da rede WiFi: " + WiFi.localIP().toString());

  configTime(-3 * 3600, 0, "pool.ntp.org");
  
  xTaskCreatePinnedToCore(distanceTask, "Distance Task", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(telegramTask, "Telegram", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(otaTask, "OTA Task", 8192, NULL, 1, NULL, 1);
}

void loop() {
  // empty – all logic is in tasks
}
