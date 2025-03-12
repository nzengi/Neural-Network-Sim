#ifndef NEURAL_NEUROMODULATION_H
#define NEURAL_NEUROMODULATION_H

#include "core/neuron.h"

typedef struct {
    double dopamine;
    double serotonin;
    double noradrenaline;
    double acetylcholine;
    double baseline_da;
    double baseline_5ht;
    double baseline_na;
    double baseline_ach;
} NeuromodulationParams;

void update_neuromodulators(Neuron* neuron, NeuromodulationParams* params,
                            double dt);
void process_reward(Neuron* neuron, NeuromodulationParams* params,
                    double reward);

#endif