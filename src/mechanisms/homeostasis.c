#include "mechanisms/homeostasis.h"

#include <math.h>

void update_homeostasis(Neuron* neuron, HomeostasisParams* params, double dt) {
    // Spike rate'i hesapla (son dt süresinde spike var mı?)
    double current_rate = neuron->last_spike_time < dt ? 1.0 / dt : 0.0;

    // Hedef rate ile karşılaştır
    double rate_error = params->target_rate - current_rate;

    // Adaptasyon akımını ayarla
    neuron->adaptation_current += params->adaptation_rate * rate_error * dt;

    // Sınırla
    if (neuron->adaptation_current < 0) {
        neuron->adaptation_current = 0;
    } else if (neuron->adaptation_current > 5.0) {
        neuron->adaptation_current = 5.0;
    }
}

void regulate_energy(Neuron* neuron, HomeostasisParams* params, double dt) {
    // Her spike'ta enerji harcanır (adaptation_current artar)
    if (neuron->last_spike_time < dt) {
        neuron->adaptation_current += 0.5;
    }

    // Zamanla enerji yenilenir (adaptation_current azalır)
    double energy_error = params->energy_baseline - neuron->adaptation_current;
    neuron->adaptation_current += params->recovery_rate * energy_error * dt;
}