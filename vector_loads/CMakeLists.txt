cmake_minimum_required(VERSION 3.16)

project(simd_benchmark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(benchmark_DIR
    "C:/Users/vivek/Documents/github/benchmark/build"
)

set(BENCHMARK_ENABLE_TESTING OFF)

add_subdirectory(
    "C:/Users/vivek/Documents/github/benchmark"
    benchmark_build
)

add_executable(simd_benchmark
    vector_loads.cpp
)

target_link_libraries(simd_benchmark
    PRIVATE
    benchmark::benchmark
)

# Enable optimizations + AVX2
target_compile_options(simd_benchmark
    PRIVATE
    -O3
    -mavx2
)