#include <unity.h>
#include "../include/neural_sim.h"
#include "../src/core/network.h"

static Network* test_network;
static SimulationConfig* test_config;

void setUp(void) {
    // Set up test configuration
    test_config = calloc(1, sizeof(SimulationConfig));
    test_config->network.num_pyramidal = 10;
    test_config->network.num_inhibitory = 2;
    test_config->network.dt = 0.0001;
    test_config->network.connection_rate = 0.1;
    
    // Create test network
    test_network = create_network(test_config->network);
}

void tearDown(void) {
    if (test_network) {
        destroy_network(test_network);
    }
    if (test_config) {
        free(test_config);
    }
}

void test_network_creation(void) {
    TEST_ASSERT_NOT_NULL(test_network);
    TEST_ASSERT_EQUAL_INT(10, test_network->config.num_pyramidal);
    TEST_ASSERT_EQUAL_INT(2, test_network->config.num_inhibitory);
    TEST_ASSERT_NOT_NULL(test_network->pyramidal_neurons);
    TEST_ASSERT_NOT_NULL(test_network->inhibitory_neurons);
}

void test_network_connectivity(void) {
    int connection_count = 0;
    int total_possible = test_config->network.num_pyramidal * 
                        test_config->network.num_pyramidal;
    
    for (int i = 0; i < test_config->network.num_pyramidal; i++) {
        for (int j = 0; j < test_config->network.num_pyramidal; j++) {
            if (test_network->connection_matrix[i * MAX_NEURONS + j]) {
                connection_count++;
            }
        }
    }
    
    double actual_rate = (double)connection_count / total_possible;
    TEST_ASSERT_FLOAT_WITHIN(0.05, test_config->network.connection_rate, actual_rate);
}

void test_network_update(void) {
    double initial_freq = test_network->population_freq_p;
    update_network(test_network, 0.0);
    TEST_ASSERT_NOT_EQUAL(initial_freq, test_network->population_freq_p);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_network_creation);
    RUN_TEST(test_network_connectivity);
    RUN_TEST(test_network_update);
    return UNITY_END();
}