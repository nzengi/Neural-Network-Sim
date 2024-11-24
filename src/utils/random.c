#include <math.h>
#include "random.h"

// PCG Random Number Generator implementation
void init_random(RandomState* state, uint64_t seed) {
    state->state = seed;
    state->inc = (seed << 1) | 1;
}

static uint32_t pcg32_random(RandomState* state) {
    uint64_t oldstate = state->state;
    state->state = oldstate * 6364136223846793005ULL + state->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

double random_uniform(RandomState* state) {
    return (double)pcg32_random(state) / UINT32_MAX;
}

double random_normal(RandomState* state, double mean, double std) {
    // Box-Muller transform
    double u1 = random_uniform(state);
    double u2 = random_uniform(state);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mean + z0 * std;
}

double random_exponential(RandomState* state, double lambda) {
    return -log(random_uniform(state)) / lambda;
}

int random_poisson(RandomState* state, double lambda) {
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;
    
    do {
        k++;
        p *= random_uniform(state);
    } while (p > L);
    
    return k - 1;
}

void random_shuffle(RandomState* state, void* array, size_t n, size_t size) {
    char* arr = (char*)array;
    char temp[size];
    
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = (size_t)(random_uniform(state) * (i + 1));
        memcpy(temp, arr + j * size, size);
        memcpy(arr + j * size, arr + i * size, size);
        memcpy(arr + i * size, temp, size);
    }
}