#ifndef NEURAL_NEURON_H
#define NEURAL_NEURON_H

#include <stdbool.h>
#include <stdio.h>

// Forward declaration
struct Dendrite;  // dendrite.h'ı include etmek yerine forward declaration
                  // kullanıyoruz

#define MAX_DENDRITES 20

typedef struct {
    double v_resting;
    double v_threshold;
    double v_reset;
    double g_leak;
    double tau_m;
    double refractory_period;
    double c_m;
} NeuronParams;

typedef struct {
    double dopamine;
    double serotonin;
    double noradrenaline;
    double acetylcholine;
} Neuromodulators;

typedef struct {
    double membrane_potential;     // Membran potansiyeli
    double calcium_concentration;  // Kalsiyum konsantrasyonu
    double adaptation_current;     // Adaptasyon akımı
    double refractory_time;        // Refractory period
    double last_spike_time;        // Son spike zamanı
    bool is_inhibitory;            // İnhibitör nöron mu?
} Neuron;

// Function declarations
Neuron* create_neuron(NeuronParams params, bool is_inhibitory);
void destroy_neuron(Neuron* neuron);
void update_neuron(Neuron* neuron, double dt);
bool check_spike(Neuron* neuron);
void init_neuron(Neuron* neuron, bool is_inhibitory);

#endif