#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <omp.h>

#include "core/network.h"
#include "core/neuron.h"
#include "mechanisms/plasticity.h"
#include "mechanisms/neuromodulation.h"
#include "mechanisms/homeostasis.h"
#include "utils/config.h"
#include "utils/random.h"
#include "utils/logger.h"

// Command line options structure
typedef struct {
    char* config_file;
    char* output_dir;
    int random_seed;
    bool verbose;
    bool use_gpu;
    double duration;
    int num_threads;
} CommandLineOptions;

// Function prototypes
void print_usage(const char* program_name);
void parse_command_line(int argc, char* argv[], CommandLineOptions* options);
void initialize_simulation(SimulationConfig* config, Network** network, Logger** logger);
void run_simulation(Network* network, SimulationConfig* config, Logger* logger);
void cleanup_simulation(Network* network, Logger* logger, SimulationConfig* config);
void print_progress(double current_time, double total_time);
void handle_signal(int signal);

// Global variables for signal handling
volatile bool simulation_running = true;

int main(int argc, char* argv[]) {
    Network* network = NULL;
    Logger* logger = NULL;
    SimulationConfig* config = NULL;
    CommandLineOptions options = {
        .config_file = "config/default_config.ini",
        .output_dir = "output",
        .random_seed = -1,
        .verbose = false,
        .use_gpu = false,
        .duration = -1.0,
        .num_threads = 1
    };

    // Parse command line arguments
    parse_command_line(argc, argv, &options);

    // Set up signal handling
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Load configuration
    config = load_config(options.config_file);
    if (!config) {
        fprintf(stderr, "Failed to load configuration from %s\n", options.config_file);
        return 1;
    }

    // Override config with command line options
    if (options.random_seed != -1) {
        config->random_seed = options.random_seed;
    }
    if (options.duration > 0) {
        config->simulation_duration = options.duration;
    }
    config->verbose = options.verbose;
    config->use_gpu = options.use_gpu;
    strcpy(config->output_dir, options.output_dir);

    // Initialize random number generator
    if (config->random_seed == -1) {
        config->random_seed = time(NULL);
    }
    RandomState rng;
    init_random(&rng, config->random_seed);

    // Set up OpenMP
    if (options.num_threads > 0) {
        omp_set_num_threads(options.num_threads);
    }

    // Initialize simulation components
    try {
        initialize_simulation(config, &network, &logger);
    } catch (const char* error) {
        fprintf(stderr, "Initialization failed: %s\n", error);
        return 1;
    }

    // Log simulation start
    log_message(logger, LOG_INFO, "Starting simulation with configuration:");
    log_message(logger, LOG_INFO, "  Random seed: %d", config->random_seed);
    log_message(logger, LOG_INFO, "  Duration: %.2f seconds", config->simulation_duration);
    log_message(logger, LOG_INFO, "  Number of neurons: %d pyramidal, %d inhibitory",
               config->network.num_pyramidal, config->network.num_inhibitory);

    // Run simulation
    clock_t start_time = clock();
    run_simulation(network, config, logger);
    clock_t end_time = clock();

    // Log simulation end
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    log_message(logger, LOG_INFO, "Simulation completed in %.2f seconds (CPU time)", cpu_time);

    // Cleanup
    cleanup_simulation(network, logger, config);

    return 0;
}

void parse_command_line(int argc, char* argv[], CommandLineOptions* options) {
    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"output", required_argument, 0, 'o'},
        {"seed", required_argument, 0, 's'},
        {"verbose", no_argument, 0, 'v'},
        {"gpu", no_argument, 0, 'g'},
        {"duration", required_argument, 0, 'd'},
        {"threads", required_argument, 0, 't'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "c:o:s:vgd:t:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 'c':
                options->config_file = optarg;
                break;
            case 'o':
                options->output_dir = optarg;
                break;
            case 's':
                options->random_seed = atoi(optarg);
                break;
            case 'v':
                options->verbose = true;
                break;
            case 'g':
                options->use_gpu = true;
                break;
            case 'd':
                options->duration = atof(optarg);
                break;
            case 't':
                options->num_threads = atoi(optarg);
                break;
            case 'h':
                print_usage(argv[0]);
                exit(0);
            case '?':
                print_usage(argv[0]);
                exit(1);
        }
    }
}

void initialize_simulation(SimulationConfig* config, Network** network, Logger** logger) {
    // Create output directory
    mkdir(config->output_dir, 0755);

    // Initialize logger
    *logger = create_logger(config->output_dir, config->verbose ? LOG_DEBUG : LOG_INFO);
    if (!*logger) {
        throw "Failed to create logger";
    }

    // Initialize network
    *network = create_network(config->network);
    if (!*network) {
        throw "Failed to create network";
    }

    // Initialize plasticity parameters
    for (int i = 0; i < config->network.num_pyramidal; i++) {
        Neuron* neuron = &(*network)->pyramidal_neurons[i];
        for (int d = 0; d < neuron->num_dendrites; d++) {
            Dendrite* dendrite = &neuron->dendrites[d];
            for (int s = 0; s < dendrite->num_synapses; s++) {
                update_meta_plasticity(&dendrite->synapses[s], &config->plasticity);
            }
        }
    }

    // Initialize homeostasis
    for (int i = 0; i < config->network.num_pyramidal; i++) {
        update_homeostasis(&(*network)->pyramidal_neurons[i], &config->homeostasis);
    }
    for (int i = 0; i < config->network.num_inhibitory; i++) {
        update_homeostasis(&(*network)->inhibitory_neurons[i], &config->homeostasis);
    }
}

void run_simulation(Network* network, SimulationConfig* config, Logger* logger) {
    double current_time = 0.0;
    int progress_interval = (int)(config->simulation_duration / 100.0 / config->network.dt);
    int step_count = 0;

    while (simulation_running && current_time < config->simulation_duration) {
        // Update network state
        update_network(network, current_time);

        // Update plasticity and homeostasis
        #pragma omp parallel for if(config->use_gpu)
        for (int i = 0; i < config->network.num_pyramidal; i++) {
            Neuron* neuron = &network->pyramidal_neurons[i];
            update_homeostasis(neuron, &config->homeostasis);
            update_neuromodulators(neuron, &config->neuromodulation);
            
            for (int d = 0; d < neuron->num_dendrites; d++) {
                Dendrite* dendrite = &neuron->dendrites[d];
                for (int s = 0; s < dendrite->num_synapses; s++) {
                    update_plasticity(&dendrite->synapses[s], &config->plasticity);
                }
            }
        }

        // Log state periodically
        if (step_count % config->save_interval == 0) {
            log_network_state(logger, network, current_time);
        }

        // Update progress display
        if (step_count % progress_interval == 0) {
            print_progress(current_time, config->simulation_duration);
        }

        current_time += config->network.dt;
        step_count++;
    }

    printf("\nSimulation %s at time %.2f seconds\n",
           simulation_running ? "completed" : "interrupted",
           current_time);
}

void cleanup_simulation(Network* network, Logger* logger, SimulationConfig* config) {
    if (network) destroy_network(network);
    if (logger) {
        flush_logs(logger);
        destroy_logger(logger);
    }
    if (config) free(config);
}

void print_progress(double current_time, double total_time) {
    int width = 50;
    double progress = current_time / total_time;
    int pos = width * progress;

    printf("\rProgress: [");
    for (int i = 0; i < width; i++) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %.1f%%", progress * 100.0);
    fflush(stdout);
}

void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n\n", program_name);
    printf("Options:\n");
    printf("  -c, --config FILE     Configuration file (default: config/default_config.ini)\n");
    printf("  -o, --output DIR      Output directory (default: output)\n");
    printf("  -s, --seed N          Random seed (default: time-based)\n");
    printf("  -v, --verbose         Enable verbose logging\n");
    printf("  -g, --gpu            Use GPU acceleration\n");
    printf("  -d, --duration N      Simulation duration in seconds\n");
    printf("  -t, --threads N       Number of OpenMP threads\n");
    printf("  -h, --help           Display this help message\n\n");
}

void handle_signal(int signal) {
    printf("\nReceived signal %d, preparing to terminate...\n", signal);
    simulation_running = false;
}