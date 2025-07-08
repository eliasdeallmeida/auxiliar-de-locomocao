# Auxiliar de Locomoção
Projeto auxiliar de locomoção para deficientes visuais desenvolvido em C++ para gravação no microcontrolador ESP32.

## Componentes
- 1 ESP32;
- 1 cabo micro USB;
- 1 LED azul;
- 1 botão;
- 1 sensor de distância HC-SR04;
- 1 motor de vibração;
- 1 módulo MP3;
- 1 alto falante;
- 15 jumper macho-macho;
- 2 jumper macho-fêmea;

## Montagem do circuito
- Posicionar devidamente o ESP32, o LED, o botão, o sensor de distância, o motor de vibração e o módulo MP3 na protoboard;
- GND do ESP32 ligado na protoboard com jumper macho-macho;
- Pino comum do LED verde (Power) ligado no GND do ESP32 com jumper macho-macho;
- Pino positivo do LED verde (Power) ligado no GPIO13 do ESP32 com jumper macho-macho;
- Botão (Power) ligado no GND do ESP32 com jumper macho-macho;
- Botão (Power) ligado no GPIO12 do ESP32 com jumper macho-macho;
- VIN do ESP32 ligado na protoboard com jumper macho-macho;
- VCC do sensor de distância ligado no VIN do ESP32 com jumper macho-macho;
- Trigger do sensor de distância ligado no GPIO5 do ESP32 com jumper macho-fêmea;
- Echo do sensor de distância ligado no GPIO18 do ESP32 com jumper macho-fêmea;
- GND do sensor de distância ligado no GND do ESP32 com jumper macho-macho;
- IN do motor de vibração ligado no GPIO25 do ESP32 com jumper macho-macho;
- VCC do motor de vibração ligado no VIN do ESP32 com jumper macho-macho;
- GND do motor de vibração ligado no GND do ESP32 com jumper macho-macho;
- VCC do módulo MP3 ligado no VIN do ESP32 com jumper macho-macho;
- RX do módulo MP3 ligado no GPIO26 do ESP32 com jumper macho-macho;
- TX do módulo MP3 ligado no GPIO27 do ESP32 com jumper macho-macho;
- GND do módulo MP3 ligado no GND do ESP32 com jumper macho-macho;
- Ligar os pinos do alto falante no módulo MP3;
