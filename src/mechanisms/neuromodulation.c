#include "mechanisms/neuromodulation.h"

#include <math.h>

void update_neuromodulators(Neuron* neuron, NeuromodulationParams* params,
                            double dt) {
    // Nöronun spike atıp atmadığını kontrol et
    bool has_spiked = neuron->last_spike_time < dt;

    // Dopamin güncellemesi
    if (has_spiked) {
        params->dopamine += 0.1;
    }
    params->dopamine =
        params->baseline_da + (params->dopamine - params->baseline_da) *
                                  exp(-dt / 1000.0);  // τ_DA = 1000ms

    // Serotonin güncellemesi
    if (has_spiked && neuron->is_inhibitory) {
        params->serotonin += 0.05;
    }
    params->serotonin =
        params->baseline_5ht + (params->serotonin - params->baseline_5ht) *
                                   exp(-dt / 2000.0);  // τ_5HT = 2000ms

    // Noradrenalin güncellemesi
    if (has_spiked) {
        params->noradrenaline += 0.2;
    }
    params->noradrenaline =
        params->baseline_na + (params->noradrenaline - params->baseline_na) *
                                  exp(-dt / 500.0);  // τ_NA = 500ms

    // Asetilkolin güncellemesi
    if (has_spiked && !neuron->is_inhibitory) {
        params->acetylcholine += 0.15;
    }
    params->acetylcholine =
        params->baseline_ach + (params->acetylcholine - params->baseline_ach) *
                                   exp(-dt / 1500.0);  // τ_ACh = 1500ms

    // Nöromodülatörlerin nöron üzerindeki etkisi
    neuron->adaptation_current *= (1.0 - 0.1 * params->dopamine);
    if (neuron->adaptation_current < 0) neuron->adaptation_current = 0;
}

void process_reward(Neuron* neuron, NeuromodulationParams* params,
                    double reward) {
    double reward_prediction_error = reward - params->dopamine;

    // Nöromodülatör seviyelerini güncelle
    params->dopamine += 0.1 * reward_prediction_error;
    params->serotonin += 0.1 * reward_prediction_error;
    params->noradrenaline += 0.2 * fabs(reward_prediction_error);

    // Nöron adaptasyonunu güncelle
    if (reward_prediction_error > 0) {
        neuron->adaptation_current *= 0.9;  // Pozitif pekiştirme
    } else {
        neuron->adaptation_current *= 1.1;  // Negatif pekiştirme
    }
}
