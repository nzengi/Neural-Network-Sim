cmake_minimum_required(VERSION 3.10)
project(advanced-neural-sim C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add OpenMP support
find_package(OpenMP)
if(OpenMP_C_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

# Add Math library
link_libraries(m)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Collect source files
file(GLOB_RECURSE SOURCES 
    "src/*.c"
)

# Create executable
add_executable(neural_sim ${SOURCES})

# Add tests
enable_testing()
file(GLOB TEST_SOURCES "tests/*.c")
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
