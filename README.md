# Auxiliar de Locomoção
Projeto auxiliar de locomoção para deficientes visuais desenvolvido em C++ para gravação no microcontrolador ESP32.

## Componentes
- 1 ESP32;
- 1 cabo micro USB;
- 1 LED azul;
- 1 LED verde;
- 2 botões;
- 1 sensor de distância HC-SR04;
- 1 motor de vibração;
- 17 jumper macho-macho;

## Montagem do circuito
- Posicionar devidamente o ESP32, os LEDs, os botões, o sensor de distância e o motor de vibração na protoboard;
- GND do ESP32 ligado na protoboard com jumper macho-macho;
- Pino comum do LED verde (Power) ligado no GND do ESP32 com jumper macho-macho;
- Pino positivo do LED verde (Power) ligado no GPIO13 do ESP32 com jumper macho-macho;
- Botão (Power) ligado no GND do ESP32 com jumper macho-macho;
- Botão (Power) ligado no GPIO12 do ESP32 com jumper macho-macho;
- VIN do ESP32 ligado na protoboard com jumper macho-macho;
- VCC do sensor de distância ligado no VIN do ESP32 com jumper macho-macho;
- Trigger do sensor de distância ligado no GPIO27 do ESP32 com jumper macho-macho;
- Echo do sensor de distância ligado no GPIO26 do ESP32 com jumper macho-macho;
- GND do sensor de distância ligado no GND do ESP32 com jumper macho-macho;
- IN do motor de vibração ligado no GPIO25 do ESP32 com jumper macho-macho;
- VCC do motor de vibração ligado no VIN do ESP32 com jumper macho-macho;
- GND do motor de vibração ligado no GND do ESP32 com jumper macho-macho;
- Pino comum do LED azul (Bot) ligado no GND do ESP32 com jumper macho-macho;
- Pino positivo do LED azul (Bot) ligado no GPIO32 do ESP32 com jumper macho-macho;
- Botão (Bot) ligado no GND do ESP32 com jumper macho-macho;
- Botão (Bot) ligado no GPIO33 do ESP32 com jumper macho-macho;
