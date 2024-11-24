#ifndef NEURAL_NEURON_H
#define NEURAL_NEURON_H

#include "dendrite.h"

#define MAX_DENDRITES 20

typedef struct {
    double v_resting;
    double v_threshold;
    double v_reset;
    double g_leak;
    double tau_m;
    double refractory_period;
    double c_m;
} NeuronParams;

typedef struct {
    double dopamine;
    double serotonin;
    double noradrenaline;
    double acetylcholine;
} Neuromodulators;

typedef struct {
    NeuronParams params;
    double membrane_potential;
    double last_spike_time;
    Dendrite* dendrites;
    int num_dendrites;
    Neuromodulators modulators;
    bool is_inhibitory;
    double adaptation_current;
    double calcium_concentration;
} Neuron;

Neuron* create_neuron(NeuronParams params, bool is_inhibitory);
void destroy_neuron(Neuron* neuron);
void update_neuron(Neuron* neuron, double dt);
bool check_spike(Neuron* neuron);

#endif