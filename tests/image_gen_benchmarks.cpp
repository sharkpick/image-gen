#include <iostream>
#include <benchmark/benchmark.h>
#include "../src/image.hpp"

class Setup {
    Setup() {
        Magick::InitializeMagick(nullptr);
    }
public:
    static void PerformSetup() {
        static Setup setup;
    }
};

static void BM_CreateEmptyRandomImage(benchmark::State& state) {
    Setup::PerformSetup();
    for (auto _ : state) auto img = RandomImage();
}

static void BM_CreateSingleIterationRandomImage(benchmark::State& state) {
    Setup::PerformSetup();
    for (auto _ : state) auto img = RandomImage(RandomImageConfiguration{});
}

BENCHMARK(BM_CreateEmptyRandomImage);
BENCHMARK(BM_CreateSingleIterationRandomImage);

BENCHMARK_MAIN();