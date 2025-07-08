#include "mp3_player.h"
#include "DFRobotDFPlayerMini.h"

#define MP3_RX_PIN 27
#define MP3_TX_PIN 26

static DFRobotDFPlayerMini mp3;
static HardwareSerial mp3Serial(2); // UART2
static VoiceGender currentGender = MALE;

void initMP3Player() {
    mp3Serial.begin(9600, SERIAL_8N1, MP3_RX_PIN, MP3_TX_PIN);
    if (!mp3.begin(mp3Serial)) {
        Serial.println("[MP3] Falha ao inicializar DFPlayer Mini!");
        return;
    }
    mp3.volume(25); // Volume de 0 a 30
}

void playProximityAudio(int fileNumber) {
    // 1 = próximo (h), 2 = aproximando (h), 3 = distante (h)
    // 4 = próximo (m), 5 = aproximando (m), 6 = distante (m)
    mp3.play(fileNumber); // Toca o arquivo correspondente
}

void setVoiceGender(VoiceGender gender) {
    currentGender = gender;
}

VoiceGender getVoiceGender() {
    return currentGender;
} 