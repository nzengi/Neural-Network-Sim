#define _USE_MATH_DEFINES  // M_PI için
#include "utils/random.h"

#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// PCG Random Number Generator implementation
void init_random(RandomState* state, uint64_t seed) {
    state->state = seed;
    state->inc = (seed << 1) | 1;
}

// PCG random number generator
uint32_t pcg32(RandomState* state) {
    uint64_t oldstate = state->state;
    state->state = oldstate * 6364136223846793005ULL + state->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

double random_double(RandomState* state) {
    return (double)pcg32(state) / (double)UINT32_MAX;
}

double random_uniform(RandomState* state) {
    return (double)pcg32(state) / (double)UINT32_MAX;
}

double random_normal(RandomState* state) {
    // Box-Muller transform
    double u1 = random_uniform(state);
    double u2 = random_uniform(state);
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
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

int random_int(RandomState* state, int min, int max) {
    return min + (pcg32(state) % (max - min + 1));
}

void random_shuffle(RandomState* state, void* array, size_t n, size_t size) {
    char* arr = (char*)array;
    char temp[256];  // Temporary buffer for swapping

    for (size_t i = n - 1; i > 0; i--) {
        size_t j = random_int(state, 0, i);
        if (i != j) {
            memcpy(temp, arr + i * size, size);
            memcpy(arr + i * size, arr + j * size, size);
            memcpy(arr + j * size, temp, size);
        }
    }
}