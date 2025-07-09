#ifndef DISTANCE_CONFIG_H
#define DISTANCE_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

// Valores padrão dos limites de distância (em cm)
#define DEFAULT_PROXIMO 10.0
#define DEFAULT_APROXIMANDO 20.0
#define DEFAULT_DISTANTE 30.0

// Estrutura para armazenar os limites de distância
struct DistanceLimits {
    float proximo;
    float aproximando;
    float distante;
};

// Funções para gerenciar a configuração de distância
void initDistanceConfig();
DistanceLimits getDistanceLimits();
void setDistanceLimits(float proximo, float aproximando, float distante);
void resetDistanceLimits();
bool validateDistanceLimits(float proximo, float aproximando, float distante);

#endif 