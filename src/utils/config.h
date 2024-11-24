#ifndef NEURAL_CONFIG_H
#define NEURAL_CONFIG_H

#include <stdbool.h>
#include "../core/network.h"
#include "../mechanisms/plasticity.h"
#include "../mechanisms/neuromodulation.h"
#include "../mechanisms/homeostasis.h"

typedef struct {
    NetworkConfig network;
    PlasticityParams plasticity;
    NeuromodulationParams neuromodulation;
    HomeostasisParams homeostasis;
    char* config_file;
    char* output_dir;
    bool verbose;
    bool use_gpu;
    int random_seed;
    double simulation_duration;
    int save_interval;
} SimulationConfig;

SimulationConfig* load_config(const char* filename);
void save_config(SimulationConfig* config, const char* filename);
void print_config(SimulationConfig* config);
void validate_config(SimulationConfig* config);

#endif