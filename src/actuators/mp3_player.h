#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include <Arduino.h>

enum VoiceGender {
    MALE,
    FEMALE
};

void initMP3Player();
void playProximityAudio(int nivel); // 1,2,3 masculino; 4,5,6 feminino
void setVoiceGender(VoiceGender gender);
VoiceGender getVoiceGender();

#endif 