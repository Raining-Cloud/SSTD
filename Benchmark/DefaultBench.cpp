#include <benchmark/benchmark.h>
#include "Containers/Vector.h"

#include <vector>

static void SSTDVector(benchmark::State& state) {
  for (auto _ : state)
  {
    SSTD::Vector<int> v;
    benchmark::DoNotOptimize(v);
  }
}

static void STDVector(benchmark::State& state) {
  for (auto _ : state)
  {
    std::vector<int> v;
    benchmark::DoNotOptimize(v);
  }
}

BENCHMARK(SSTDVector);
BENCHMARK(STDVector);

BENCHMARK_MAIN();