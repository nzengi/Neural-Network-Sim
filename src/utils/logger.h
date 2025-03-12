#ifndef NEURAL_LOGGER_H
#define NEURAL_LOGGER_H

#include <stdio.h>

#include "core/network.h"

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR } LogLevel;

typedef struct Logger {
    LogLevel level;
    char* log_directory;
    FILE* log_file;   // Ana log dosyası için
    FILE* data_file;  // Veri kayıtları için
    double last_save_time;
} Logger;

// Function declarations
Logger* create_logger(const char* log_dir, LogLevel level);
void destroy_logger(Logger* logger);
void log_message(Logger* logger, LogLevel level, const char* format, ...);
void log_network_state(Logger* logger, Network* network, double time);
void log_neuron_state(Logger* logger, Neuron* neuron, int id, double time);
void flush_logs(Logger* logger);

#endif