#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <LittleFS.h>

#include "hardware/hardware_setup.h"
#include "logs/log_manager.h"
#include "power/power_manager.h"
#include "tasks/distance_task.h"
#include "tasks/button_task.h"

#define BOT_TOKEN "BOT_TOKEN"
#define CHAT_ID "CHAT_ID"

#define SSID "SSID"
#define PASSWORD "PASSWORD"

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
    Serial.println("Comando recebido: " + text);

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
      welcome += "/logs";
      bot.sendMessage(chat_id, welcome, "");
    }

    else if (text == "/logs") {
      awaitingLogCount = true;
      bot.sendMessage(chat_id, "Quantos logs deseja exibir?\nInsira qualquer coisa para exibir a quantidade padrão (20 logs)", "");
    }
    else if (text == "/clearlogs") {
      bool success = clearLogs();
      if (success) {
        bot.sendMessage(chat_id, "✅ Todos os logs foram apagados e o contador foi reiniciado.", "");
      } else {
        bot.sendMessage(chat_id, "❌ Erro ao apagar os logs.", "");
      }
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

  if (!isWakingFromDeepSleep()) {
    Serial.println("[BOOT] Normal startup. Going to sleep...");
    enterDeepSleep();
  }

  Serial.println("[BOOT] Woke up from Deep Sleep");
  turnOnPowerLED();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando com o WiFi...");
  }

  Serial.print("IP adquirido da rede WiFi: ");
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin(true)) {
    Serial.println("[ERRO] LittleFS não pôde ser montado.");
  }

  configTime(-3 * 3600, 0, "pool.ntp.org");

  xTaskCreatePinnedToCore(distanceTask, "Distance Task", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(buttonTask, "Button Task", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(telegramTask, "Telegram", 8192, NULL, 1, NULL, 1);
}

void loop() {
  // empty – all logic is in tasks
}
