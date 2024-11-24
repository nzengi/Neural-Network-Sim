#ifndef NEURAL_HOMEOSTASIS_H
#define NEURAL_HOMEOSTASIS_H

#include "../core/neuron.h"

typedef struct {
    double target_rate;
    double adaptation_rate;
    double calcium_target;
    double conductance_scaling;
    double metabolic_cost;
    double energy_baseline;
    double recovery_rate;
} HomeostasisParams;

void update_homeostasis(Neuron* neuron, HomeostasisParams* params);
void scale_synaptic_strengths(Neuron* neuron, HomeostasisParams* params);
void regulate_intrinsic_excitability(Neuron* neuron, HomeostasisParams* params);
void manage_energy_resources(Neuron* neuron, HomeostasisParams* params);

#endif