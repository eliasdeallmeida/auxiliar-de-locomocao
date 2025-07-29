# Auxiliar de Locomoção
Projeto auxiliar de locomoção para deficientes visuais desenvolvido em C++ para gravação no microcontrolador ESP32.

## Componentes
- ESP32;
- LED;
- Botão;
- Sensor de distância HC-SR04;
- Motor de vibração;
- Módulo MP3;
- Alto falante;
- Cabos jumper;

## Montagem do circuito
- Posicionar devidamente o ESP32, o LED, o botão, o sensor de distância, o motor de vibração e o módulo MP3 na protoboard;
- GND do ESP32 ligado na protoboard com jumper;
- Pino comum do LED (Power) ligado no GND do ESP32 com jumper;
- Pino positivo do LED (Power) ligado no GPIO13 do ESP32 com jumper;
- Botão (Power) ligado no GND do ESP32 com jumper;
- Botão (Power) ligado no GPIO12 do ESP32 com jumper;
- VIN do ESP32 ligado na protoboard com jumper;
- VCC do sensor de distância ligado no VIN do ESP32 com jumper;
- Trigger do sensor de distância ligado no GPIO5 do ESP32 com jumper macho-fêmea;
- Echo do sensor de distância ligado no GPIO18 do ESP32 com jumper macho-fêmea;
- GND do sensor de distância ligado no GND do ESP32 com jumper;
- IN do motor de vibração ligado no GPIO25 do ESP32 com jumper;
- VCC do motor de vibração ligado no VIN do ESP32 com jumper;
- GND do motor de vibração ligado no GND do ESP32 com jumper;
- VCC do módulo MP3 ligado no VIN do ESP32 com jumper;
- RX do módulo MP3 ligado no GPIO26 do ESP32 com jumper;
- TX do módulo MP3 ligado no GPIO27 do ESP32 com jumper;
- GND do módulo MP3 ligado no GND do ESP32 com jumper;
- Ligar os pinos do alto falante no módulo MP3;
