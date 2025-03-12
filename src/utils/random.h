#ifndef NEURAL_RANDOM_H
#define NEURAL_RANDOM_H

#include <stddef.h>  // size_t için
#include <stdint.h>

// Thread-local random state
typedef struct {
    uint64_t state;
    uint64_t inc;
} RandomState;

// Function declarations
void init_random(RandomState* state, uint64_t seed);
double random_uniform(RandomState* state);
double random_normal(RandomState* state);
double random_exponential(RandomState* state, double lambda);
int random_poisson(RandomState* state, double lambda);
int random_int(RandomState* state, int min, int max);
void random_shuffle(RandomState* state, void* array, size_t n, size_t size);

#endif