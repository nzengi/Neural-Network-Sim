#include <stdlib.h>
#include <math.h>
#include "dendrite.h"

Dendrite* create_dendrite(int num_synapses) {
    Dendrite* dendrite = (Dendrite*)malloc(sizeof(Dendrite));
    if (!dendrite) return NULL;
    
    dendrite->local_potential = 0.0;
    dendrite->calcium_concentration = 0.0;
    dendrite->nmda_conductance = 0.0;
    dendrite->coupling_strength = 1.0;
    
    dendrite->synapses = (Synapse*)malloc(num_synapses * sizeof(Synapse));
    dendrite->num_synapses = num_synapses;
    
    for (int i = 0; i < num_synapses; i++) {
        dendrite->synapses[i] = *create_synapse(-1, -1); // Initialize with invalid IDs
    }
    
    return dendrite;
}

void update_dendrite(Dendrite* dendrite, double dt) {
    // Update synapses
    for (int i = 0; i < dendrite->num_synapses; i++) {
        update_synapse(&dendrite->synapses[i], dt);
    }
    
    // Update calcium concentration
    dendrite->calcium_concentration *= (1.0 - dt / 20.0); // τ_Ca = 20ms
    
    // Update NMDA conductance based on calcium
    double ca_threshold = 0.5;
    if (dendrite->calcium_concentration > ca_threshold) {
        dendrite->nmda_conductance = 1.0 / (1.0 + exp(-(dendrite->calcium_concentration - ca_threshold)));
    } else {
        dendrite->nmda_conductance *= (1.0 - dt / 100.0);
    }
}