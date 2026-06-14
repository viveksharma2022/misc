#include <benchmark/benchmark.h>
#include <vector>
#include <immintrin.h>
#include <numeric>

static constexpr size_t N = 1 << 20; // 1 million elements

template <typename T>
inline T horizontal_add(__m256 vec){
    
    vec = _mm256_hadd_ps(vec, vec); // Add adjacent pairs
    vec = _mm256_hadd_ps(vec, vec); // Add the results again

    // extract the lowest and highest 128 bits and add them together
    __m128 low = _mm256_castps256_ps128(vec); // Get the lower 128 bits
    __m128 high = _mm256_extractf128_ps(vec, 1); // Get the upper 128 bits
    __m128 sum = _mm_add_ss(low, high); // Add the first element of low and high
    return _mm_cvtss_f32(sum); // Extract the final sum
}

template <typename T>
static std::vector<T> make_data(){
    std::vector<T> data(N);
    std::iota(data.begin(), data.end(), 0); // Fill with 0, 1, 2, ...
    return data;
}

// Benchmark for normal load
template <typename T>
static void normal_load(benchmark::State& state){
    auto data = make_data<T>();
    
    for(auto _: state){
        T sum = 0;
        for(size_t i = 0; i < data.size(); ++i){
            sum += data[i]; // Normal load
        }
        benchmark::DoNotOptimize(sum); // Prevent optimization
    }

    state.SetBytesProcessed(state.iterations() * data.size() * sizeof(T));
}

// Benchmark for vectorized load (using SIMD)
template <typename T>
static void vectorized_load(benchmark::State& state){
    auto data = make_data<T>();

    for(auto _: state){
        size_t i = 0;
        // Process 256 bits (32 bytes) at a time for float (8 floats) and 512 bits (64 bytes) for double (8 doubles)
        __m256 vec_sum = _mm256_setzero_ps();

        for(; i + 8 < data.size(); i += 8){
            __m256 vec = _mm256_loadu_ps(&data[i]); // Vectorized load
            vec_sum = _mm256_add_ps(vec_sum, vec); // Vectorized addition
        }

        // Horizontal add to sum the vector elements
        T sum = horizontal_add<T>(vec_sum);
        
        // Handle remaining elements
        for(; i < data.size(); ++i){
            sum += data[i];
        }
        benchmark::DoNotOptimize(sum); // Prevent optimization
    }
    
    state.SetBytesProcessed(state.iterations() * data.size() * sizeof(T));
}


static constexpr int ITERS = 1000;

BENCHMARK(normal_load<float>)->Iterations(ITERS);;
BENCHMARK(vectorized_load<float>)->Iterations(ITERS);

BENCHMARK_MAIN();