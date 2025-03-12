#ifndef NEURAL_HOMEOSTASIS_H
#define NEURAL_HOMEOSTASIS_H

#include "core/neuron.h"

typedef struct {
    double target_rate;        // Hedef ateşleme hızı
    double adaptation_rate;    // Adaptasyon hızı
    double energy_baseline;    // Enerji bazal seviyesi
    double recovery_rate;      // İyileşme hızı
} HomeostasisParams;

void update_homeostasis(Neuron* neuron, HomeostasisParams* params, double dt);
void regulate_energy(Neuron* neuron, HomeostasisParams* params, double dt);

#endif