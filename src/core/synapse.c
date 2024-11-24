#include <stdlib.h>
#include <math.h>
#include "synapse.h"

Synapse* create_synapse(int pre_id, int post_id) {
    Synapse* synapse = (Synapse*)malloc(sizeof(Synapse));
    if (!synapse) return NULL;
    
    synapse->weight = 0.1 * ((double)rand() / RAND_MAX);
    synapse->last_update_time = 0.0;
    synapse->trace = 0.0;
    synapse->meta_plasticity = 1.0;
    synapse->is_active = true;
    synapse->pre_neuron_id = pre_id;
    synapse->post_neuron_id = post_id;
    synapse->conductance = 0.0;
    
    return synapse;
}

void update_synapse(Synapse* synapse, double dt) {
    // Decay synaptic trace
    synapse->trace *= (1.0 - dt / 20.0); // τ_trace = 20ms
    
    // Decay conductance
    synapse->conductance *= (1.0 - dt / 5.0); // τ_syn = 5ms
}

void update_weight(Synapse* synapse, double learning_rate) {
    // Implement STDP
    double weight_change = learning_rate * synapse->trace * synapse->meta_plasticity;
    synapse->weight += weight_change;
    
    // Bound weights
    if (synapse->weight < 0.0) synapse->weight = 0.0;
    if (synapse->weight > 5.0) synapse->weight = 5.0;
    
    // Update metaplasticity
    synapse->meta_plasticity *= (1.0 + 0.1 * weight_change);
}