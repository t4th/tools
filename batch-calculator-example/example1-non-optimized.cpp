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

void FillBuffersWithRandomNumbers(FooUpdateIn * buffer, size_t count);

void UpdateFoos(const volatile FooUpdateIn * in, size_t count, volatile FooUpdateOut * out, float f)
{
    for (size_t i = 0; i < count; i++) {
        float mag = sqrt(
            in[i].m_Velocity[0] * in[i].m_Velocity[0] +
            in[i].m_Velocity[1] * in[i].m_Velocity[1]);     // (6/32)=~5.33 loop/cache line
        out[i].m_Foo = in[i].m_Foo + mag + f;            // sqrt + math = ~40x5.33= 213.33 cycles/cache line
    }
}

// data buffers
#define BUFFER_COUNT 10000000

volatile FooUpdateIn     g_input[BUFFER_COUNT]; // make it volatile so /O2 wont remove it if not used
volatile FooUpdateOut    g_output[BUFFER_COUNT];

inline void RunOptimized(const unsigned batch_size) // (BUFFER_COUNT % batch_size == 0) must be fulfilled
{
    for (size_t i = 0; i < BUFFER_COUNT / batch_size; i++)
        // prompt compiler to read memory every batch_size, instead every element
    {
        UpdateFoos(g_input, batch_size, g_output, 3.14f);
    }
}

void RunNotOptimized(void)
{
    UpdateFoos(g_input, BUFFER_COUNT, g_output, 3.14f);
}

int main(int argc, char *argv[])
{
    int run_optimazed = 0;

    if (argc == 2)
    {
        run_optimazed = atoi(argv[1]);

        if (run_optimazed) {
            std::cout << "run optimized" << std::endl;
        }
        else {
            std::cout << "run non-optimized" << std::endl;
        }
    }
    else
    {
        std::cout << "run non-optimized by default" << std::endl;
    }

    std::srand((unsigned int)std::time(nullptr));

    auto start = std::chrono::high_resolution_clock::now(); // damn, this is long

    if (run_optimazed) {
        RunOptimized(32); // batch size 32
    }
    else {
        RunNotOptimized();
    }

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout  << "time elapsed: " << elapsed.count() << std::endl;

    return 0;
}

void FillBuffersWithRandomNumbers(FooUpdateIn * buffer, size_t count)
{
    for (unsigned i = 0; i < count; i++)
    {
        buffer->m_Velocity[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        buffer->m_Velocity[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        buffer->m_Foo = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    }
}
