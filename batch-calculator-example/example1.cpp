#include <iostream>
#include <ctime>
#include <chrono>
#include <math.h>

struct FooUpdateIn {     // 12 bytes x count(32) = 384 = 64 x 6
    float m_Velocity[2];
    float m_Foo;
};

struct FooUpdateOut {    // 4 bytes x count(32) = 128 = 64 x 2
    float m_Foo;
};

// original mike acton example
void UpdateFoos(const volatile FooUpdateIn * in, size_t count, volatile FooUpdateOut * out, float f) {
    for (size_t i = 0; i < count; i++) {
        float mag = sqrt(
            in[i].m_Velocity[0] * in[i].m_Velocity[0] +
            in[i].m_Velocity[1] * in[i].m_Velocity[1]);     // (6/32)=~5.33 loop/cache line
        out[i].m_Foo = in[i].m_Foo + mag + f;            // sqrt + math = ~40x5.33= 213.33 cycles/cache line
    }
}

// test configuration
#define BATCH_SIZE   32
#define BUFFER_COUNT 10000000 // make it BUFFER_COUNT % BATCH_SIZE == 0

// data buffers
// both are volatile so /O2 wont remove it even if initialized to 0
volatile FooUpdateIn     g_input[BUFFER_COUNT];
volatile FooUpdateOut    g_output[BUFFER_COUNT];

inline void RunOptimized() {
    // prompt compiler to read memory every batch_size, instead every element
    for (size_t i = 0; i < BUFFER_COUNT / BATCH_SIZE; i++) {
        UpdateFoos(g_input, BATCH_SIZE, g_output, 3.14f);
    }
}

void RunNotOptimized(void) {
    // just do linear buffer calculation
    UpdateFoos(g_input, BUFFER_COUNT, g_output, 3.14f);
}

int main(int argc, char *argv[])
{
    bool run_optimazed = false;

    if (argc > 1) {
        run_optimazed = true;
        std::cout << "run optimized" << std::endl;
    }
    else {
        std::cout << "run non-optimized" << std::endl;
    }

    std::srand((unsigned int)std::time(nullptr));

    auto start = std::chrono::high_resolution_clock::now();

    if (run_optimazed) {
        RunOptimized();
    }
    else {
        RunNotOptimized();
    }

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout  << "time elapsed: " << elapsed.count() << std::endl;
    
    return 0;
}
