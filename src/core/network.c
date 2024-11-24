#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

Network* create_network(NetworkConfig config) {
    Network* net = (Network*)malloc(sizeof(Network));
    if (!net) {
        fprintf(stderr, "Failed to allocate network\n");
        return NULL;
    }

    net->config = config;
    
    // Allocate neurons
    net->pyramidal_neurons = (Neuron*)malloc(config.num_pyramidal * sizeof(Neuron));
    net->inhibitory_neurons = (Neuron*)malloc(config.num_inhibitory * sizeof(Neuron));
    
    // Initialize connection matrix
    int total_neurons = config.num_pyramidal + config.num_inhibitory;
    net->connection_matrix = (bool*)calloc(total_neurons * total_neurons, sizeof(bool));
    
    // Create random connections
    for (int i = 0; i < total_neurons; i++) {
        for (int j = 0; j < total_neurons; j++) {
            if (i != j && ((double)rand() / RAND_MAX) < config.connection_rate) {
                net->connection_matrix[i * total_neurons + j] = true;
            }
        }
    }
    
    // Initialize population frequencies
    net->population_freq_p = 0.0;
    net->population_freq_i = 0.0;
    
    // Open output files
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/pyramidal_activity.txt", config.output_dir);
    net->output_files[0] = fopen(filename, "w");
    
    snprintf(filename, sizeof(filename), "%s/inhibitory_activity.txt", config.output_dir);
    net->output_files[1] = fopen(filename, "w");
    
    return net;
}

void destroy_network(Network* net) {
    if (net) {
        free(net->pyramidal_neurons);
        free(net->inhibitory_neurons);
        free(net->connection_matrix);
        
        for (int i = 0; i < 2; i++) {
            if (net->output_files[i]) {
                fclose(net->output_files[i]);
            }
        }
        
        free(net);
    }
}

void update_network(Network* net, double time) {
    // Update pyramidal neurons
    for (int i = 0; i < net->config.num_pyramidal; i++) {
        update_neuron(&net->pyramidal_neurons[i], net->config.dt);
        if (check_spike(&net->pyramidal_neurons[i])) {
            net->population_freq_p += 1.0;
        }
    }
    
    // Update inhibitory neurons
    for (int i = 0; i < net->config.num_inhibitory; i++) {
        update_neuron(&net->inhibitory_neurons[i], net->config.dt);
        if (check_spike(&net->inhibitory_neurons[i])) {
            net->population_freq_i += 1.0;
        }
    }
    
    // Decay population frequencies
    net->population_freq_p *= (1.0 - net->config.dt);
    net->population_freq_i *= (1.0 - net->config.dt);
}
