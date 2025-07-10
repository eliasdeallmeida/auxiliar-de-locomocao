#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <Arduino.h>

enum VoiceGender {
    MALE,
    FEMALE
};

void initMP3Player();
void playProximityAudio(int nivel); // 1,2,3 masculino; 4,5,6 feminino
void playWifiConnectionAudio(); // Toca áudio de "conectando wifi"
void setVoiceGender(VoiceGender gender);
VoiceGender getVoiceGender();
void setVolume(int volume); // Volume de 0 a 30
int getCurrentVolume(); // Retorna o volume atual (0 a 30)

#endif
