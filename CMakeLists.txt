cmake_minimum_required(VERSION 3.10)
project(advanced_neural_simulation C)

# Compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenMP REQUIRED)
find_package(GSL REQUIRED)

# Add compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")
if(OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

# Source files
set(SOURCES
    src/main.c
    src/core/dendrite.c
    src/core/network.c
    src/core/neuron.c
    src/core/synapse.c
    src/mechanisms/plasticity.c
    src/mechanisms/neuromodulation.c
    src/mechanisms/homeostasis.c
    src/utils/config.c
    src/utils/logger.c
    src/utils/random.c
)

# Create executable
add_executable(neural_sim ${SOURCES})

# Link libraries
target_link_libraries(neural_sim
    m
    GSL::gsl
    GSL::gslcblas
    ${OpenMP_C_LIBRARIES}
)

# Include directories
target_include_directories(neural_sim PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${GSL_INCLUDE_DIRS}
)

# Tests (disabled for now)
# add_executable(test_dendrite tests/test_dendrite.c)
# target_link_libraries(test_dendrite neural_sim)
