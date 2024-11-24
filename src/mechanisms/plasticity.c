#include <math.h>
#include "plasticity.h"

void update_stdp(Synapse* synapse, Neuron* pre, Neuron* post, PlasticityParams* params) {
    double dt = post->last_spike_time - pre->last_spike_time;
    double dw = 0.0;
    
    // Classic STDP with exponential windows
    if (fabs(dt) < params->stdp_window) {
        if (dt > 0) {
            // Post after pre - potentiation
            dw = params->stdp_potentiation * exp(-dt / params->stdp_window);
        } else {
            // Pre after post - depression
            dw = -params->stdp_depression * exp(dt / params->stdp_window);
        }
    }
    
    // Modulate weight change by meta-plasticity
    dw *= synapse->meta_plasticity;
    
    // Apply weight change
    synapse->weight += params->learning_rate * dw;
    
    // Bound weights
    if (synapse->weight < 0.0) synapse->weight = 0.0;
    if (synapse->weight > 5.0) synapse->weight = 5.0;
    
    // Update synaptic trace
    synapse->trace = synapse->trace * 0.9 + 0.1;
}

void update_homeostatic_plasticity(Neuron* neuron, PlasticityParams* params) {
    // Calculate activity difference from target
    double activity_diff = neuron->calcium_concentration - params->target_activity;
    
    // Scale synaptic weights based on activity
    for (int i = 0; i < neuron->num_dendrites; i++) {
        Dendrite* dend = &neuron->dendrites[i];
        for (int j = 0; j < dend->num_synapses; j++) {
            Synapse* syn = &dend->synapses[j];
            
            // Homeostatic scaling
            syn->weight *= (1.0 - params->homeostatic_rate * activity_diff);
            
            // Ensure weights remain bounded
            if (syn->weight < 0.0) syn->weight = 0.0;
            if (syn->weight > 5.0) syn->weight = 5.0;
        }
    }
}

void update_meta_plasticity(Synapse* synapse, PlasticityParams* params) {
    // Update metaplasticity based on recent activity
    double recent_activity = synapse->trace;
    
    // Sliding threshold for plasticity
    synapse->meta_plasticity += params->meta_rate * (1.0 - recent_activity);
    
    // Bound metaplasticity
    if (synapse->meta_plasticity < 0.0) synapse->meta_plasticity = 0.0;
    if (synapse->meta_plasticity > 2.0) synapse->meta_plasticity = 2.0;
}

void consolidate_weights(Synapse* synapse, PlasticityParams* params) {
    // Weight consolidation through metaplasticity
    if (synapse->weight > 0.8) {
        synapse->meta_plasticity *= (1.0 - params->consolidation_rate);
    } else if (synapse->weight < 0.2) {
        synapse->meta_plasticity *= (1.0 + params->consolidation_rate);
    }
}
