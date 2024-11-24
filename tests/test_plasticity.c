#include <unity.h>
#include "../src/mechanisms/plasticity.h"

static PlasticityParams test_params;
static Synapse* test_synapse;
static Neuron *pre_neuron, *post_neuron;

void setUp(void) {
    test_params = (PlasticityParams){
        .learning_rate = 0.01,
        .stdp_window = 0.020,
        .stdp_potentiation = 1.0,
        .stdp_depression = 1.0,
        .homeostatic_rate = 0.001,
        .target_activity = 0.1,
        .consolidation_rate = 0.1,
        .meta_rate = 0.001
    };
    
    test_synapse = create_synapse(0, 1);
    pre_neuron = create_neuron((NeuronParams){0}, false);
    post_neuron = create_neuron((NeuronParams){0}, false);
}

void tearDown(void) {
    if (test_synapse) destroy_synapse(test_synapse);
    if (pre_neuron) destroy_neuron(pre_neuron);
    if (post_neuron) destroy_neuron(post_neuron);
}

void test_stdp_potentiation(void) {
    // Simulate post-before-pre timing
    pre_neuron->last_spike_time = 0.010;
    post_neuron->last_spike_time = 0.015;
    
    double initial_weight = test_synapse->weight;
    update_stdp(test_synapse, pre_neuron, post_neuron, &test_params);
    
    TEST_ASSERT_GREATER_THAN(initial_weight, test_synapse->weight);
}

void test_stdp_depression(void) {
    // Simulate pre-before-post timing
    pre_neuron->last_spike_time = 0.015;
    post_neuron->last_spike_time = 0.010;
    
    double initial_weight = test_synapse->weight;
    update_stdp(test_synapse, pre_neuron, post_neuron, &test_params);
    
    TEST_ASSERT_LESS_THAN(initial_weight, test_synapse->weight);
}

void test_weight_bounds(void) {
    test_synapse->weight = 10.0;
    update_stdp(test_synapse, pre_neuron, post_neuron, &test_params);
    TEST_ASSERT_LESS_OR_EQUAL(5.0, test_synapse->weight);
    
    test_synapse->weight = -1.0;
    update_stdp(test_synapse, pre_neuron, post_neuron, &test_params);
    TEST_ASSERT_GREATER_OR_EQUAL(0.0, test_synapse->weight);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stdp_potentiation);
    RUN_TEST(test_stdp_depression);
    RUN_TEST(test_weight_bounds);
    return UNITY_END();
}
