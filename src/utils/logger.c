#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include "logger.h"

Logger* create_logger(const char* log_dir, LogLevel min_level) {
    Logger* logger = malloc(sizeof(Logger));
    if (!logger) return NULL;

    // Create log directory if it doesn't exist
    mkdir(log_dir, 0755);

    // Create log filename with timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/sim_%04d%02d%02d_%02d%02d%02d.log",
             log_dir, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);

    logger->log_file = fopen(filename, "w");
    if (!logger->log_file) {
        free(logger);
        return NULL;
    }

    // Initialize logger properties
    logger->min_level = min_level;
    logger->console_output = true;
    logger->log_directory = strdup(log_dir);
    logger->buffer_size = 4096;
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
    if (level < logger->min_level) return;

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    
    // Get level string
    const char* level_str;
    switch (level) {
        case LOG_DEBUG:   level_str = "DEBUG"; break;
        case LOG_INFO:    level_str = "INFO"; break;
        case LOG_WARNING: level_str = "WARNING"; break;
        case LOG_ERROR:   level_str = "ERROR"; break;
        default:          level_str = "UNKNOWN";
    }

    // Format timestamp and level
    char timestamp[20];
    snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d %02d:%02d:%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);

    // Format message
    va_list args;
    va_start(args, format);
    char message[1024];
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    // Write to log file
    fprintf(logger->log_file, "[%s] %s: %s\n", timestamp, level_str, message);
    if (logger->console_output) {
        fprintf(stdout, "[%s] %s: %s\n", timestamp, level_str, message);
    }
    
    // Flush if error
    if (level == LOG_ERROR) {
        fflush(logger->log_file);
    }
}

void log_network_state(Logger* logger, Network* network, double time) {
    // Create data filename if needed
    if (!logger->data_file) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/network_state.dat", logger->log_directory);
        logger->data_file = fopen(filename, "w");
        if (!logger->data_file) return;
        
        // Write header
        fprintf(logger->data_file, "Time\tPopFreqP\tPopFreqI\n");
    }

    // Log network state
    fprintf(logger->data_file, "%.6f\t%.6f\t%.6f\n",
            time, network->population_freq_p, network->population_freq_i);

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
        snprintf(filename, sizeof(filename), "%s/neuron_states.dat", logger->log_directory);
        neuron_file = fopen(filename, "w");
        if (!neuron_file) return;
    }

    // Write header if needed
    if (!header_written) {
        fprintf(neuron_file, "Time\tID\tVoltage\tCalcium\tAdaptation\n");
        header_written = true;
    }

    // Log neuron state
    fprintf(neuron_file, "%.6f\t%d\t%.6f\t%.6f\t%.6f\n",
            time, id, neuron->membrane_potential,
            neuron->calcium_concentration, neuron->adaptation_current);
}

void flush_logs(Logger* logger) {
    if (logger->log_file) fflush(logger->log_file);
    if (logger->data_file) fflush(logger->data_file);
}