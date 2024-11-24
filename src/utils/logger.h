#ifndef NEURAL_LOGGER_H
#define NEURAL_LOGGER_H

#include <stdio.h>
#include "../core/network.h"

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

typedef struct {
    FILE* log_file;
    FILE* data_file;
    LogLevel min_level;
    bool console_output;
    char* log_directory;
    int buffer_size;
    double last_save_time;
} Logger;

Logger* create_logger(const char* log_dir, LogLevel min_level);
void destroy_logger(Logger* logger);
void log_message(Logger* logger, LogLevel level, const char* format, ...);
void log_network_state(Logger* logger, Network* network, double time);
void log_neuron_state(Logger* logger, Neuron* neuron, int id, double time);
void flush_logs(Logger* logger);

#endif