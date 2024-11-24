#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "config.h"

#define MAX_LINE_LENGTH 1024

static void parse_line(char* line, char* key, char* value) {
    char* equals = strchr(line, '=');
    if (equals) {
        *equals = '\0';
        strcpy(key, line);
        strcpy(value, equals + 1);
        // Trim whitespace
        char* end = key + strlen(key) - 1;
        while (end > key && isspace(*end)) *end-- = '\0';
        char* start = value;
        while (*start && isspace(*start)) start++;
        strcpy(value, start);
        end = value + strlen(value) - 1;
        while (end > value && isspace(*end)) *end-- = '\0';
    }
}

SimulationConfig* load_config(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening config file: %s\n", strerror(errno));
        return NULL;
    }

    SimulationConfig* config = calloc(1, sizeof(SimulationConfig));
    char line[MAX_LINE_LENGTH];
    char key[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        parse_line(line, key, value);
        
        // Network parameters
        if (strcmp(key, "num_pyramidal") == 0) {
            config->network.num_pyramidal = atoi(value);
        } else if (strcmp(key, "num_inhibitory") == 0) {
            config->network.num_inhibitory = atoi(value);
        }
        // Add more parameter parsing as needed...
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
