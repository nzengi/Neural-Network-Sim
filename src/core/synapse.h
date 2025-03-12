#ifndef NEURAL_SYNAPSE_H
#define NEURAL_SYNAPSE_H

#include <stdbool.h>
#include <stdio.h>

// Forward declarations only - no includes
struct Neuron;
struct Dendrite;

typedef struct Synapse {
    double weight;
    double last_update_time;
    double trace;
    double meta_plasticity;
    bool is_active;
    int pre_neuron_id;
    int post_neuron_id;
    double conductance;
} Synapse;

// Function declarations
Synapse* create_synapse(int pre_id, int post_id);
void destroy_synapse(Synapse* synapse);
void update_synapse(Synapse* synapse, double dt);
void update_weight(Synapse* synapse, double learning_rate);

#endif