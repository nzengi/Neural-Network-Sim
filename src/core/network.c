#include "network.h"

#include <errno.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

Network* create_network(NetworkConfig config) {
    Network* net = (Network*)malloc(sizeof(Network));
    if (!net) {
        fprintf(stderr, "Failed to allocate network\n");
        return NULL;
    }

    net->config = config;

    // Allocate neurons
    net->pyramidal_neurons =
        (Neuron*)calloc(config.num_pyramidal, sizeof(Neuron));
    if (!net->pyramidal_neurons) {
        fprintf(stderr, "Failed to allocate pyramidal neurons\n");
        free(net);
        return NULL;
    }

    net->inhibitory_neurons =
        (Neuron*)calloc(config.num_inhibitory, sizeof(Neuron));
    if (!net->inhibitory_neurons) {
        fprintf(stderr, "Failed to allocate inhibitory neurons\n");
        free(net->pyramidal_neurons);
        free(net);
        return NULL;
    }

    // Initialize connection matrix
    int total_neurons = config.num_pyramidal + config.num_inhibitory;
    net->connection_matrix =
        (bool*)calloc(total_neurons * total_neurons, sizeof(bool));
    if (!net->connection_matrix) {
        fprintf(stderr, "Failed to allocate connection matrix\n");
        free(net->inhibitory_neurons);
        free(net->pyramidal_neurons);
        free(net);
        return NULL;
    }

    // Initialize output files array
    memset(net->output_files, 0, sizeof(net->output_files));

    // Create output directory if it doesn't exist
    if (mkdir(config.output_dir, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "Failed to create output directory\n");
        destroy_network(net);
        return NULL;
    }

    // Initialize population frequencies
    net->population_freq_p = 0.0;
    net->population_freq_i = 0.0;

    // Initialize neurons
    for (int i = 0; i < config.num_pyramidal; i++) {
        init_neuron(&net->pyramidal_neurons[i], false);
    }
    for (int i = 0; i < config.num_inhibitory; i++) {
        init_neuron(&net->inhibitory_neurons[i], true);
    }

    // Create random connections
    for (int i = 0; i < total_neurons; i++) {
        for (int j = 0; j < total_neurons; j++) {
            if (i != j &&
                ((double)rand() / RAND_MAX) < config.connection_rate) {
                net->connection_matrix[i * total_neurons + j] = true;
            }
        }
    }

    // Open output files
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/pyramidal_activity.txt",
             config.output_dir);
    net->output_files[0] = fopen(filename, "w");

    snprintf(filename, sizeof(filename), "%s/inhibitory_activity.txt",
             config.output_dir);
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
// Update pyramidal neurons in parallel
#pragma omp parallel for
    for (int i = 0; i < net->config.num_pyramidal; i++) {
        update_neuron(&net->pyramidal_neurons[i], net->config.dt);
        if (check_spike(&net->pyramidal_neurons[i])) {
#pragma omp atomic
            net->population_freq_p += 1.0;
        }
    }

// Update inhibitory neurons in parallel
#pragma omp parallel for
    for (int i = 0; i < net->config.num_inhibitory; i++) {
        update_neuron(&net->inhibitory_neurons[i], net->config.dt);
        if (check_spike(&net->inhibitory_neurons[i])) {
#pragma omp atomic
            net->population_freq_i += 1.0;
        }
    }

    // Decay population frequencies
    net->population_freq_p *= (1.0 - net->config.dt);
    net->population_freq_i *= (1.0 - net->config.dt);
}

void save_network_state(Network* net, double time) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/network_state_%.3f.txt",
             net->config.output_dir, time);

    FILE* file = fopen(filename, "w");
    if (!file) return;

    fprintf(file, "Time: %.3f\n", time);
    fprintf(file, "Population frequency (pyramidal): %.3f Hz\n",
            net->population_freq_p);
    fprintf(file, "Population frequency (inhibitory): %.3f Hz\n",
            net->population_freq_i);

    fclose(file);
}
