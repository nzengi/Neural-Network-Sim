#include <math.h>
#include "neuromodulation.h"

void update_neuromodulators(Neuron* neuron, NeuromodulationParams* params) {
    // Decay towards baseline
    neuron->modulators.dopamine = neuron->modulators.dopamine * (1.0 - params->decay_rate) +
                                 params->baseline_dopamine * params->decay_rate;
    
    neuron->modulators.serotonin = neuron->modulators.serotonin * (1.0 - params->decay_rate) +
                                  params->baseline_serotonin * params->decay_rate;
    
    neuron->modulators.noradrenaline = neuron->modulators.noradrenaline * (1.0 - params->decay_rate) +
                                      params->baseline_noradrenaline * params->decay_rate;
    
    neuron->modulators.acetylcholine = neuron->modulators.acetylcholine * (1.0 - params->decay_rate) +
                                      params->baseline_acetylcholine * params->decay_rate;
}

void process_reward(Neuron* neuron, double reward, NeuromodulationParams* params) {
    // Update dopamine based on reward
    double reward_prediction_error = reward - neuron->modulators.dopamine;
    neuron->modulators.dopamine += params->reward_strength * reward_prediction_error;
    
    // Secondary effects on other modulators
    neuron->modulators.serotonin += 0.1 * reward_prediction_error;
    neuron->modulators.noradrenaline += 0.2 * fabs(reward_prediction_error);
}

void modulate_plasticity(Synapse* synapse, Neuromodulators* modulators) {
    // Calculate modulation factor
    double modulation = (modulators->dopamine / modulators->acetylcholine) *
                       sqrt(modulators->noradrenaline);
    
    // Apply modulation to synaptic parameters
    synapse->meta_plasticity *= modulation;
    synapse->trace *= (1.0 + 0.1 * (modulation - 1.0));
}

void update_attention(Neuron* neuron, NeuromodulationParams* params) {
    // Modulate attention through noradrenaline and acetylcholine
    double attention_level = neuron->modulators.noradrenaline * 
                           neuron->modulators.acetylcholine;
    
    // Adjust neuronal parameters based on attention
    neuron->params.v_threshold *= (1.0 - params->attention_modulation * (attention_level - 1.0));
    neuron->params.g_leak *= (1.0 + params->attention_modulation * (attention_level - 1.0));
}
