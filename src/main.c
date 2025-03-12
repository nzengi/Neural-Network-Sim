#include <errno.h>
#include <getopt.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "core/network.h"
#include "utils/config.h"

// Command line options structure
typedef struct {
    char* config_file;
    char* output_dir;
} CommandLineOptions;

// Function declarations
static void parse_command_line(int argc, char** argv,
                               CommandLineOptions* options);
static void print_usage(const char* program_name);

int main(int argc, char** argv) {
// Initialize OpenMP
#ifdef _OPENMP
    omp_set_num_threads(omp_get_max_threads());
    printf("Using OpenMP with %d threads\n", omp_get_max_threads());
#endif

    // Parse command line options
    CommandLineOptions options = {0};
    options.config_file = "config/default_config.ini";
    options.output_dir = "output";
    parse_command_line(argc, argv, &options);

    // Load configuration
    SimulationConfig* config = load_config(options.config_file);
    if (!config) {
        fprintf(stderr, "Failed to load configuration\n");
        return EXIT_FAILURE;
    }

    // Override output directory if specified
    if (options.output_dir) {
        config->network.output_dir = strdup(options.output_dir);
    }

    // Create network
    Network* network = create_network(config->network);
    if (!network) {
        fprintf(stderr, "Failed to create network\n");
        destroy_config(config);
        return EXIT_FAILURE;
    }

    // Run simulation
    double time = 0.0;
    while (time < config->network.simulation_time) {
        printf("\rSimulation progress: %.1f%%",
               (time / config->network.simulation_time) * 100.0);
        fflush(stdout);

        update_network(network, time);
        save_network_state(network, time);
        time += config->network.dt;
    }
    printf("\nSimulation completed\n");

    // Cleanup
    destroy_network(network);
    destroy_config(config);
    return EXIT_SUCCESS;
}

static void parse_command_line(int argc, char** argv,
                               CommandLineOptions* options) {
    int opt;
    while ((opt = getopt(argc, argv, "c:o:h")) != -1) {
        switch (opt) {
            case 'c':
                options->config_file = optarg;
                break;
            case 'o':
                options->output_dir = optarg;
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                exit(EXIT_SUCCESS);
        }
    }
}

static void print_usage(const char* program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf(
        "  -c <file>    Configuration file (default: "
        "config/default_config.ini)\n");
    printf("  -o <dir>     Output directory (default: output)\n");
    printf("  -h           Show this help message\n");
}