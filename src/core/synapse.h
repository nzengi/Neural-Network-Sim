// network.h
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdbool.h>
#include "neuron.h"
#include "dendrite.h"
#include "synapse.h"

#define MAX_NEURONS 505
#define MAX_CONNECTIONS 50000

typedef struct {
    int num_pyramidal;
    int num_inhibitory;
    double dt;
    double simulation_time;
    double connection_rate;
    char* output_dir;
} NetworkConfig;

typedef struct {
    NetworkConfig config;
    Neuron* pyramidal_neurons;
    Neuron* inhibitory_neurons;
    bool* connection_matrix;
    double population_freq_p;
    double population_freq_i;
    FILE* output_files[3];
} Network;

Network* create_network(NetworkConfig config);
void destroy_network(Network* net);
void update_network(Network* net, double time);
void save_network_state(Network* net, double time);

#endif

// neuron.h
#ifndef NEURAL_NEURON_H
#define NEURAL_NEURON_H

#include "dendrite.h"

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
    NeuronParams params;
    double membrane_potential;
    double last_spike_time;
    Dendrite* dendrites;
    int num_dendrites;
    Neuromodulators modulators;
    bool is_inhibitory;
    double adaptation_current;
    double calcium_concentration;
} Neuron;

Neuron* create_neuron(NeuronParams params, bool is_inhibitory);
void destroy_neuron(Neuron* neuron);
void update_neuron(Neuron* neuron, double dt);
bool check_spike(Neuron* neuron);

#endif

// dendrite.h
#ifndef NEURAL_DENDRITE_H
#define NEURAL_DENDRITE_H

#include "synapse.h"

#define MAX_SYNAPSES_PER_DENDRITE 100

typedef struct {
    double local_potential;
    double calcium_concentration;
    double nmda_conductance;
    Synapse* synapses;
    int num_synapses;
    double coupling_strength;
} Dendrite;

Dendrite* create_dendrite(int num_synapses);
void destroy_dendrite(Dendrite* dendrite);
void update_dendrite(Dendrite* dendrite, double dt);
double compute_local_potential(Dendrite* dendrite);

#endif

// synapse.h
#ifndef NEURAL_SYNAPSE_H
#define NEURAL_SYNAPSE_H

#include <stdbool.h>

typedef struct {
    double weight;
    double last_update_time;
    double trace;
    double meta_plasticity;
    bool is_active;
    int pre_neuron_id;
    int post_neuron_id;
    double conductance;
} Synapse;

Synapse* create_synapse(int pre_id, int post_id);
void destroy_synapse(Synapse* synapse);
void update_synapse(Synapse* synapse, double dt);
void update_weight(Synapse* synapse, double learning_rate);

#endif