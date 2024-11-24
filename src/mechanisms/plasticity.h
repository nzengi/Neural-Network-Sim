#ifndef NEURAL_PLASTICITY_H
#define NEURAL_PLASTICITY_H

#include "../core/synapse.h"
#include "../core/neuron.h"

typedef struct {
    double learning_rate;
    double stdp_window;
    double stdp_potentiation;
    double stdp_depression;
    double homeostatic_rate;
    double target_activity;
    double consolidation_rate;
    double meta_rate;
} PlasticityParams;

void update_stdp(Synapse* synapse, Neuron* pre, Neuron* post, PlasticityParams* params);
void update_homeostatic_plasticity(Neuron* neuron, PlasticityParams* params);
void update_meta_plasticity(Synapse* synapse, PlasticityParams* params);
void consolidate_weights(Synapse* synapse, PlasticityParams* params);

#endif