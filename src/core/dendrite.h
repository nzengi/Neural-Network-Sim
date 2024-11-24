#ifndef NEURAL_DENDRITE_H
#define NEURAL_DENDRITE_H

#include "synapse.h"

#define MAX_SYNAPSES_PER_DENDRITE 100

typedef struct {
    double local_potential;
    double calcium_concentration;
    double nmda_conductance;
    Synapse* synapses;
    int num_synapses;
    double coupling_strength;
} Dendrite;

Dendrite* create_dendrite(int num_synapses);
void destroy_dendrite(Dendrite* dendrite);
void update_dendrite(Dendrite* dendrite, double dt);
double compute_local_potential(Dendrite* dendrite);

#endif