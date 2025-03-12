#ifndef NEURAL_PLASTICITY_H
#define NEURAL_PLASTICITY_H

#include "core/neuron.h"

typedef struct {
    double learning_rate;
    double target_rate;
    double adaptation_rate;
} PlasticityParams;

void update_homeostatic_plasticity(Neuron* neuron, PlasticityParams* params,
                                   double dt);
void update_synaptic_plasticity(Neuron* pre, Neuron* post, double* weight,
                                PlasticityParams* params);

#endif