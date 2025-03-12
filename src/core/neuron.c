#include "neuron.h"

#include <math.h>
#include <stdlib.h>

#include "dendrite.h"  // Dendrite yapısının tam tanımı için
#include "dendrite.h"

void init_neuron(Neuron* neuron, bool is_inhibitory) {
    neuron->membrane_potential = -65.0;  // Dinlenme potansiyeli
    neuron->calcium_concentration = 0.0;
    neuron->adaptation_current = 0.0;
    neuron->refractory_time = 0.0;
    neuron->last_spike_time = -1000.0;  // Başlangıçta spike yok
    neuron->is_inhibitory = is_inhibitory;
}

void update_neuron(Neuron* neuron, double dt) {
    // Basit Integrate-and-Fire model
    if (neuron->refractory_time > 0) {
        neuron->refractory_time -= dt;
        return;
    }

    // Random input current (test için)
    double input_current = ((double)rand() / RAND_MAX) * 20.0 - 10.0;

    // Update membrane potential
    double tau = 20.0;  // Time constant
    double dv = (-65.0 - neuron->membrane_potential) / tau + input_current;
    neuron->membrane_potential += dv * dt;
}

bool check_spike(Neuron* neuron) {
    if (neuron->membrane_potential >= -55.0) {  // Spike threshold
        neuron->membrane_potential = -75.0;     // Reset potential
        neuron->refractory_time = 2.0;          // Refractory period
        neuron->last_spike_time = 0.0;          // Reset spike time
        return true;
    }
    return false;
}
