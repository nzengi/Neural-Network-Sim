#include <unity.h>
#include "../src/core/dendrite.h"

static Dendrite* test_dendrite;

void setUp(void) {
    test_dendrite = create_dendrite(10); // Create dendrite with 10 synapses
}

void tearDown(void) {
    if (test_dendrite) destroy_dendrite(test_dendrite);
}

void test_dendrite_creation(void) {
    TEST_ASSERT_NOT_NULL(test_dendrite);
    TEST_ASSERT_EQUAL_INT(10, test_dendrite->num_synapses);
    TEST_ASSERT_NOT_NULL(test_dendrite->synapses);
}

void test_local_potential(void) {
    // Set up test synapses
    test_dendrite->synapses[0].weight = 1.0;
    test_dendrite->synapses[0].conductance = 0.5;
    test_dendrite->synapses[0].is_active = true;
    
    double potential = compute_local_potential(test_dendrite);
    TEST_ASSERT_GREATER_THAN(0.0, potential);
}

void test_calcium_dynamics(void) {
    test_dendrite->calcium_concentration = 1.0;
    update_dendrite(test_dendrite, 0.001);
    TEST_ASSERT_LESS_THAN(1.0, test_dendrite->calcium_concentration);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_dendrite_creation);
    RUN_TEST(test_local_potential);
    RUN_TEST(test_calcium_dynamics);
    return UNITY_END();
}
