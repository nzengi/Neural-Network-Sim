/**
 * @file neural_sim.h
 * @brief Main header file for the Neural Network Simulation project
 * @version 2.0.0
 * @date 2024-02-24
 * 
 * This header provides the main interface for the neural network simulation.
 * It includes all necessary components and defines the public API.
 */

#ifndef NEURAL_SIM_H
#define NEURAL_SIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <signal.h>

// Version information
#define NEURAL_SIM_VERSION_MAJOR 2
#define NEURAL_SIM_VERSION_MINOR 0
#define NEURAL_SIM_VERSION_PATCH 0
#define NEURAL_SIM_VERSION_STRING "2.0.0"

// System constants
#define MAX_NEURONS 505
#define MAX_SYNAPSES 50000
#define MAX_DENDRITES 20
#define MAX_FILENAME_LENGTH 256
#define BUFFER_SIZE 4096

// Error codes
typedef enum {
    NS_SUCCESS = 0,
    NS_ERROR_MEMORY = -1,
    NS_ERROR_FILE = -2,
    NS_ERROR_CONFIG = -3,
    NS_ERROR_PARAM = -4,
    NS_ERROR_STATE = -5,
    NS_ERROR_INIT = -6,
    NS_ERROR_RUNTIME = -7
} NeuralSimError;

// Forward declarations of core structures
struct Network;
struct Neuron;
struct Dendrite;
struct Synapse;

// Main simulation interface
typedef struct {
    // Simulation state
    bool initialized;
    bool running;
    double current_time;
    double end_time;
    
    // Components
    struct Network* network;
    struct Logger* logger;
    struct RandomState* rng;
    
    // Configuration
    struct SimulationConfig* config;
    
    // Statistics
    size_t step_count;
    double computation_time;
    double last_save_time;
    
    // Control flags
    bool pause_requested;
    bool stop_requested;
    bool save_requested;
} NeuralSimulation;

// Callback function types
typedef void (*ProgressCallback)(double progress, void* user_data);
typedef void (*StateCallback)(const NeuralSimulation* sim, void* user_data);
typedef void (*ErrorCallback)(NeuralSimError error, const char* message, void* user_data);

// Configuration structure for callbacks
typedef struct {
    ProgressCallback progress_cb;
    StateCallback state_cb;
    ErrorCallback error_cb;
    void* user_data;
    double progress_interval;
    double save_interval;
} SimulationCallbacks;

// Main API functions

/**
 * @brief Initialize the neural simulation
 * @param config_file Path to the configuration file
 * @param callbacks Optional callback functions
 * @return Pointer to initialized simulation or NULL on error
 */
NeuralSimulation* ns_init(const char* config_file, const SimulationCallbacks* callbacks);

/**
 * @brief Run the simulation for the specified duration
 * @param sim Pointer to simulation instance
 * @param duration Simulation duration in seconds (-1 for config default)
 * @return Error code
 */
NeuralSimError ns_run(NeuralSimulation* sim, double duration);

/**
 * @brief Pause the running simulation
 * @param sim Pointer to simulation instance
 * @return Error code
 */
NeuralSimError ns_pause(NeuralSimulation* sim);

/**
 * @brief Resume a paused simulation
 * @param sim Pointer to simulation instance
 * @return Error code
 */
NeuralSimError ns_resume(NeuralSimulation* sim);

/**
 * @brief Stop the simulation and cleanup
 * @param sim Pointer to simulation instance
 * @return Error code
 */
NeuralSimError ns_stop(NeuralSimulation* sim);

/**
 * @brief Save the current simulation state
 * @param sim Pointer to simulation instance
 * @param filename Optional filename (NULL for default name)
 * @return Error code
 */
NeuralSimError ns_save_state(NeuralSimulation* sim, const char* filename);

/**
 * @brief Load a previously saved simulation state
 * @param sim Pointer to simulation instance
 * @param filename State file to load
 * @return Error code
 */
NeuralSimError ns_load_state(NeuralSimulation* sim, const char* filename);

/**
 * @brief Get the last error message
 * @return Error message string
 */
const char* ns_get_last_error(void);

/**
 * @brief Get version information
 * @return Version string
 */
const char* ns_get_version(void);

// Utility functions

/**
 * @brief Convert error code to string
 * @param error Error code
 * @return Error description string
 */
const char* ns_error_string(NeuralSimError error);

/**
 * @brief Check if CUDA/GPU support is available
 * @return true if GPU support is available
 */
bool ns_has_gpu_support(void);

/**
 * @brief Get system information string
 * @return System information string
 */
const char* ns_get_system_info(void);

// Analysis functions

/**
 * @brief Calculate network statistics
 * @param sim Pointer to simulation instance
 * @param stats Pointer to statistics structure to fill
 * @return Error code
 */
NeuralSimError ns_calculate_statistics(const NeuralSimulation* sim, struct NetworkStatistics* stats);

/**
 * @brief Export simulation data to various formats
 * @param sim Pointer to simulation instance
 * @param format Export format (CSV, HDF5, etc.)
 * @param filename Output filename
 * @return Error code
 */
NeuralSimError ns_export_data(const NeuralSimulation* sim, const char* format, const char* filename);

// Debug/Development functions
#ifdef NEURAL_SIM_DEBUG
/**
 * @brief Enable detailed debug logging
 * @param level Debug level (0-5)
 */
void ns_set_debug_level(int level);

/**
 * @brief Dump internal state for debugging
 * @param sim Pointer to simulation instance
 * @param filename Output filename
 */
void ns_dump_state(const NeuralSimulation* sim, const char* filename);
#endif

// Macros for version checking
#define NS_VERSION_CHECK(major,minor,patch) \
    (NEURAL_SIM_VERSION_MAJOR > (major) || \
    (NEURAL_SIM_VERSION_MAJOR == (major) && NEURAL_SIM_VERSION_MINOR > (minor)) || \
    (NEURAL_SIM_VERSION_MAJOR == (major) && NEURAL_SIM_VERSION_MINOR == (minor) && \
     NEURAL_SIM_VERSION_PATCH >= (patch)))

#ifdef __cplusplus
}
#endif

#endif // NEURAL_SIM_H