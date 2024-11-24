#include <stdlib.h>
#include <math.h>
#include "neuron.h"

Neuron* create_neuron(NeuronParams params, bool is_inhibitory) {
    Neuron* neuron = (Neuron*)malloc(sizeof(Neuron));
    if (!neuron) return NULL;
    
    neuron->params = params;
    neuron->membrane_potential = params.v_resting;
    neuron->last_spike_time = -1000.0; // Large negative number
    neuron->is_inhibitory = is_inhibitory;
    
    // Initialize dendrites
    neuron->dendrites = (Dendrite*)malloc(MAX_DENDRITES * sizeof(Dendrite));
    neuron->num_dendrites = MAX_DENDRITES;
    
    for (int i = 0; i < MAX_DENDRITES; i++) {
        neuron->dendrites[i] = *create_dendrite(MAX_SYNAPSES_PER_DENDRITE);
    }
    
    // Initialize neuromodulators
    neuron->modulators.dopamine = 1.0;
    neuron->modulators.serotonin = 1.0;
    neuron->modulators.noradrenaline = 1.0;
    neuron->modulators.acetylcholine = 1.0;
    
    neuron->adaptation_current = 0.0;
    neuron->calcium_concentration = 0.0;
    
    return neuron;
}

void update_neuron(Neuron* neuron, double dt) {
    // Check if in refractory period
    if ((neuron->last_spike_time > 0) && 
        ((neuron->last_spike_time + neuron->params.refractory_period) > dt)) {
        return;
    }
    
    // Update dendrites
    double dendritic_input = 0.0;
    for (int i = 0; i < neuron->num_dendrites; i++) {
        update_dendrite(&neuron->dendrites[i], dt);
        dendritic_input += compute_local_potential(&neuron->dendrites[i]);
    }
    
    // Update membrane potential
    double dv = (-neuron->params.g_leak * (neuron->membrane_potential - neuron->params.v_resting) +
                 dendritic_input - neuron->adaptation_current) / neuron->params.c_m;
    
    neuron->membrane_potential += dv * dt;
    
    // Update calcium concentration
    neuron->calcium_concentration *= (1.0 - dt / 50.0); // τ_Ca = 50ms
    
    // Update adaptation current
    neuron->adaptation_current *= (1.0 - dt / 100.0); // τ_w = 100ms
}

bool check_spike(Neuron* neuron) {
    if (neuron->membrane_potential >= neuron->params.v_threshold) {
        neuron->membrane_potential = neuron->params.v_reset;
        neuron->last_spike_time = 0.0;
        neuron->calcium_concentration += 0.1;
        neuron->adaptation_current += 0.2;
        return true;
    }
    return false;
}
