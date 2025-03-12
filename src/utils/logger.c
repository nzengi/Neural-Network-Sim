#include "utils/logger.h"

#include <errno.h>   // errno ve EEXIST için
#include <stdarg.h>  // va_list için
#include <stdio.h>
#include <stdlib.h>  // malloc, free için
#include <string.h>  // strdup için
#include <sys/stat.h>
#include <time.h>

Logger* create_logger(const char* log_dir, LogLevel level) {
    Logger* logger = malloc(sizeof(Logger));
    if (!logger) {
        fprintf(stderr, "Failed to allocate memory for logger\n");
        return NULL;
    }

    // Create log directory if it doesn't exist
    char log_path[256];
    snprintf(log_path, sizeof(log_path), "%s/logs", log_dir);
    if (mkdir(log_path, 0755) != 0 && errno != EEXIST) {
        fprintf(stderr, "Failed to create log directory: %s\n", log_path);
        free(logger);
        return NULL;
    }

    // Initialize logger
    logger->level = level;
    logger->log_directory = strdup(log_dir);
    if (!logger->log_directory) {
        fprintf(stderr, "Failed to allocate memory for log directory path\n");
        free(logger);
        return NULL;
    }

    // Open main log file
    char log_file[256];
    snprintf(log_file, sizeof(log_file), "%s/simulation.log", log_dir);
    logger->log_file = fopen(log_file, "a");
    if (!logger->log_file) {
        fprintf(stderr, "Failed to open log file: %s\n", log_file);
        free(logger->log_directory);
        free(logger);
        return NULL;
    }

    logger->data_file = NULL;
    logger->last_save_time = 0.0;

    return logger;
}

void destroy_logger(Logger* logger) {
    if (logger) {
        if (logger->log_file) fclose(logger->log_file);
        if (logger->data_file) fclose(logger->data_file);
        free(logger->log_directory);
        free(logger);
    }
}

void log_message(Logger* logger, LogLevel level, const char* format, ...) {
    if (level < logger->level) return;

    time_t now;
    time(&now);
    char timestamp[26];
    ctime_r(&now, timestamp);
    timestamp[24] = '\0';  // Remove newline

    fprintf(logger->log_file, "[%s] ", timestamp);
    switch (level) {
        case LOG_DEBUG:
            fprintf(logger->log_file, "[DEBUG] ");
            break;
        case LOG_INFO:
            fprintf(logger->log_file, "[INFO] ");
            break;
        case LOG_WARNING:
            fprintf(logger->log_file, "[WARNING] ");
            break;
        case LOG_ERROR:
            fprintf(logger->log_file, "[ERROR] ");
            break;
    }

    va_list args;
    va_start(args, format);
    vfprintf(logger->log_file, format, args);
    va_end(args);
    fprintf(logger->log_file, "\n");

    // Flush immediately for errors
    if (level == LOG_ERROR) {
        fflush(logger->log_file);
    }
}

void log_network_state(Logger* logger, Network* network, double time) {
    // Create data filename if needed
    if (!logger->data_file) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/network_state.dat",
                 logger->log_directory);
        logger->data_file = fopen(filename, "w");
        if (!logger->data_file) return;

        // Write header
        fprintf(logger->data_file, "Time\tPopFreqP\tPopFreqI\n");
    }

    // Log network state
    fprintf(logger->data_file, "%.6f\t%.6f\t%.6f\n", time,
            network->population_freq_p, network->population_freq_i);

    // Periodic flush
    if (time - logger->last_save_time >= 1.0) {
        fflush(logger->data_file);
        logger->last_save_time = time;
    }
}

void log_neuron_state(Logger* logger, Neuron* neuron, int id, double time) {
    static bool header_written = false;
    static FILE* neuron_file = NULL;

    // Create file if needed
    if (!neuron_file) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/neuron_states.dat",
                 logger->log_directory);
        neuron_file = fopen(filename, "w");
        if (!neuron_file) return;
    }

    // Write header if needed
    if (!header_written) {
        fprintf(neuron_file, "Time\tID\tVoltage\tCalcium\tAdaptation\n");
        header_written = true;
    }

    // Log neuron state
    fprintf(neuron_file, "%.6f\t%d\t%.6f\t%.6f\t%.6f\n", time, id,
            neuron->membrane_potential, neuron->calcium_concentration,
            neuron->adaptation_current);
}

void flush_logs(Logger* logger) {
    if (logger->log_file) fflush(logger->log_file);
    if (logger->data_file) fflush(logger->data_file);
}