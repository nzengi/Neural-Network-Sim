#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdbool.h>

#include "dendrite.h"
#include "neuron.h"
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

// Network state management
void save_network_state(Network* net, double time);

#endif
