#include <unity.h>
#include "../src/utils/config.h"
#include "../src/utils/random.h"
#include "../src/utils/logger.h"

static RandomState rng;
static Logger* test_logger;
static SimulationConfig* test_config;

void setUp(void) {
    init_random(&rng, 12345);
    test_logger = create_logger("test_logs", LOG_DEBUG);
    test_config = load_config("test_config.ini");
}

void tearDown(void) {
    if (test_logger) destroy_logger(test_logger);
    if (test_config) free(test_config);
}

void test_random_distribution(void) {
    double sum = 0.0;
    int n = 1000;
    
    for (int i = 0; i < n; i++) {
        double val = random_uniform(&rng);
        TEST_ASSERT_GREATER_OR_EQUAL(0.0, val);
        TEST_ASSERT_LESS_THAN(1.0, val);
        sum += val;
    }
    
    double mean = sum / n;
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.5, mean);
}

void test_config_loading(void) {
    TEST_ASSERT_NOT_NULL(test_config);
    TEST_ASSERT_GREATER_THAN(0, test_config->network.num_pyramidal);
    TEST_ASSERT_GREATER_THAN(0.0, test_config->network.dt);
}

void test_logger_functionality(void) {
    TEST_ASSERT_NOT_NULL(test_logger);
    log_message(test_logger, LOG_INFO, "Test message");
    TEST_ASSERT_EQUAL(LOG_DEBUG, test_logger->min_level);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_random_distribution);
    RUN_TEST(test_config_loading);
    RUN_TEST(test_logger_functionality);
    return UNITY_END();
}
