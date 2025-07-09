#include "config/distance_config.h"
#include "logs/log_manager.h"

static Preferences preferences;
static const char* NAMESPACE = "distance";

void initDistanceConfig() {
    preferences.begin(NAMESPACE, false);
    
    // Verifica se já existem valores salvos, se não, usa os padrões
    if (preferences.getFloat("proximo", -1) == -1) {
        logPrintln("[CONFIG] Inicializando configurações de distância com valores padrão");
        setDistanceLimits(DEFAULT_PROXIMO, DEFAULT_APROXIMANDO, DEFAULT_DISTANTE);
    } else {
        logPrintln("[CONFIG] Configurações de distância carregadas da memória");
    }
}

DistanceLimits getDistanceLimits() {
    DistanceLimits limits;
    limits.proximo = preferences.getFloat("proximo", DEFAULT_PROXIMO);
    limits.aproximando = preferences.getFloat("aproximando", DEFAULT_APROXIMANDO);
    limits.distante = preferences.getFloat("distante", DEFAULT_DISTANTE);
    return limits;
}

void setDistanceLimits(float proximo, float aproximando, float distante) {
    if (validateDistanceLimits(proximo, aproximando, distante)) {
        preferences.putFloat("proximo", proximo);
        preferences.putFloat("aproximando", aproximando);
        preferences.putFloat("distante", distante);
        
        logPrintf("[CONFIG] Limites atualizados - Próximo: %.1f, Aproximando: %.1f, Distante: %.1f\n", 
                  proximo, aproximando, distante);
    } else {
        logPrintln("[CONFIG] Erro: Valores de distância inválidos");
    }
}

void resetDistanceLimits() {
    setDistanceLimits(DEFAULT_PROXIMO, DEFAULT_APROXIMANDO, DEFAULT_DISTANTE);
    logPrintln("[CONFIG] Limites de distância resetados para valores padrão");
}

bool validateDistanceLimits(float proximo, float aproximando, float distante) {
    // Verifica se os valores estão em ordem crescente
    if (proximo >= aproximando || aproximando >= distante) {
        return false;
    }
    
    // Verifica se os valores estão dentro de limites razoáveis (1cm a 500cm)
    if (proximo < 1.0 || proximo > 500.0 ||
        aproximando < 1.0 || aproximando > 500.0 ||
        distante < 1.0 || distante > 500.0) {
        return false;
    }
    
    return true;
} 