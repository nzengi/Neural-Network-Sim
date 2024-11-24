#include <math.h>
#include "homeostasis.h"

void update_homeostasis(Neuron* neuron, HomeostasisParams* params) {
    // Update calcium homeostasis
    double calcium_error = neuron->calcium_concentration - params->calcium_target;
    neuron->calcium_concentration -= params->adaptation_rate * calcium_error;
    
    // Update firing rate homeostasis
    double rate_error = neuron->adaptation_current - params->target_rate;
    neuron->adaptation_current -= params->adaptation_rate * rate_error;
    
    // Energy management
    manage_energy_resources(neuron, params);
}

void scale_synaptic_strengths(Neuron* neuron, HomeostasisParams* params) {
    double activity_factor = neuron->calcium_concentration / params->calcium_target;
    
    // Scale synaptic conductances
    for (int i = 0; i < neuron->num_dendrites; i++) {
        Dendrite* dend = &neuron->dendrites[i];
        for (int j = 0; j < dend->num_synapses; j++) {
            dend->synapses[j].conductance *= 
                (1.0 - params->conductance_scaling * (activity_factor - 1.0));
        }
    }
}

void regulate_intrinsic_excitability(Neuron* neuron, HomeostasisParams* params) {
    // Adjust threshold based on activity
    double activity_error = neuron->calcium_concentration - params->calcium_target;
    neuron->params.v_threshold += params->adaptation_rate * activity_error;
    
    // Adjust leak conductance
    neuron->params.g_leak *= (1.0 + params->adaptation_rate * activity_error);
}

void manage_energy_resources(Neuron* neuron, HomeostasisParams* params) {
    // Calculate energy consumption
    double energy_consumption = params->metabolic_cost * 
        (neuron->calcium_concentration + fabs(neuron->membrane_potential) +
         neuron->adaptation_current);
    
    // Update energy availability
    double energy_deficit = params->energy_baseline - energy_consumption;
    
    // Adjust neural parameters based on energy
    if (energy_deficit < 0) {
        // Energy conservation mode
        neuron->params.g_leak *= (1.0 + params->recovery_rate);
        neuron->params.v_threshold += params->recovery_rate;
    } else {
        // Normal operation mode
        neuron->params.g_leak = neuron->params.g_leak * (1.0 - params->recovery_rate) +
                               params->energy_baseline * params->recovery_rate;
        neuron->params.v_threshold = neuron->params.v_threshold * (1.0 - params->recovery_rate) +
                                   params->energy_baseline * params->recovery_rate;
    }
}