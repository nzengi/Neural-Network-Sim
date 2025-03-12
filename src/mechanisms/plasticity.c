#include "mechanisms/plasticity.h"
#include <math.h>

void update_homeostatic_plasticity(Neuron* neuron, PlasticityParams* params, double dt) {
    // Basit homeostatic plastisite: Adaptasyon akımını ayarla
    double rate_error = neuron->last_spike_time < dt ? 1.0 : 0.0;
    rate_error = params->target_rate - rate_error;
    
    neuron->adaptation_current += params->adaptation_rate * rate_error * dt;
    
    // Sınırla
    if (neuron->adaptation_current < 0) neuron->adaptation_current = 0;
    if (neuron->adaptation_current > 5.0) neuron->adaptation_current = 5.0;
}

void update_synaptic_plasticity(Neuron* pre, Neuron* post, double* weight, PlasticityParams* params) {
    // Basit STDP benzeri plastisite
    double dt = post->last_spike_time - pre->last_spike_time;
    double dw = 0.0;
    
    if (fabs(dt) < 20.0) {  // 20ms pencere
        if (dt > 0) {  // Post after pre -> strengthen
            dw = params->learning_rate * exp(-dt/10.0);
        } else {  // Pre after post -> weaken
            dw = -params->learning_rate * exp(dt/10.0);
        }
        
        *weight += dw;
        
        // Sınırla
        if (*weight < 0.0) *weight = 0.0;
        if (*weight > 2.0) *weight = 2.0;
    }
}
