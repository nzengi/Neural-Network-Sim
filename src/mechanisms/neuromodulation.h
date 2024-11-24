#ifndef NEURAL_NEUROMODULATION_H
#define NEURAL_NEUROMODULATION_H

#include "../core/neuron.h"

typedef struct {
    double baseline_dopamine;
    double baseline_serotonin;
    double baseline_noradrenaline;
    double baseline_acetylcholine;
    double decay_rate;
    double learning_modulation;
    double attention_modulation;
    double reward_strength;
} NeuromodulationParams;

void update_neuromodulators(Neuron* neuron, NeuromodulationParams* params);
void process_reward(Neuron* neuron, double reward, NeuromodulationParams* params);
void modulate_plasticity(Synapse* synapse, Neuromodulators* modulators);
void update_attention(Neuron* neuron, NeuromodulationParams* params);

#endif