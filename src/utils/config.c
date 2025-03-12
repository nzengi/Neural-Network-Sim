#include "utils/config.h"

#include <ctype.h>  // isspace için
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

static void parse_line(char* line, SimulationConfig* config) {
    char* key = line;
    char* value = strchr(line, '=');
    if (!value) return;

    // Split line into key-value pair
    *value++ = '\0';

    // Trim whitespace
    char* end = value + strlen(value) - 1;
    while (end > key && isspace(*end)) *end-- = '\0';
    while (isspace(*value)) value++;

    end = key + strlen(key) - 1;
    while (end > key && isspace(*end)) *end-- = '\0';
    while (isspace(*key)) key++;

    // Parse values based on key
    if (strcmp(key, "num_pyramidal") == 0) {
        config->network.num_pyramidal = atoi(value);
    } else if (strcmp(key, "num_inhibitory") == 0) {
        config->network.num_inhibitory = atoi(value);
    } else if (strcmp(key, "dt") == 0) {
        config->network.dt = atof(value);
    } else if (strcmp(key, "simulation_time") == 0) {
        config->network.simulation_time = atof(value);
    } else if (strcmp(key, "connection_rate") == 0) {
        config->network.connection_rate = atof(value);
    } else if (strcmp(key, "output_dir") == 0) {
        config->network.output_dir = strdup(value);
    }
}

SimulationConfig* load_config(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open config file: %s\n", filename);
        return NULL;
    }

    SimulationConfig* config =
        (SimulationConfig*)malloc(sizeof(SimulationConfig));
    if (!config) {
        fclose(file);
        return NULL;
    }

    // Set default values
    config->network.num_pyramidal = 400;
    config->network.num_inhibitory = 100;
    config->network.dt = 0.1;
    config->network.simulation_time = 1000.0;
    config->network.connection_rate = 0.1;
    config->network.output_dir = "output";

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;
        parse_line(line, config);
    }

    fclose(file);
    validate_config(config);
    return config;
}

void save_config(SimulationConfig* config, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error saving config file: %s\n", strerror(errno));
        return;
    }

    fprintf(file, "# Network Configuration\n");
    fprintf(file, "num_pyramidal=%d\n", config->network.num_pyramidal);
    fprintf(file, "num_inhibitory=%d\n", config->network.num_inhibitory);
    fprintf(file, "dt=%f\n", config->network.dt);
    fprintf(file, "simulation_time=%f\n", config->network.simulation_time);
    fprintf(file, "connection_rate=%f\n", config->network.connection_rate);
    fprintf(file, "output_dir=%s\n", config->network.output_dir);
    // Add more parameters...

    fclose(file);
}

void validate_config(SimulationConfig* config) {
    // Validate network parameters
    if (config->network.num_pyramidal <= 0) {
        fprintf(stderr, "Invalid number of pyramidal neurons\n");
        exit(1);
    }
    if (config->network.num_inhibitory <= 0) {
        fprintf(stderr, "Invalid number of inhibitory neurons\n");
        exit(1);
    }
}

void destroy_config(SimulationConfig* config) {
    if (config) {
        free(config->network.output_dir);
        free(config);
    }
}
