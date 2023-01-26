#include <benchmark/benchmark.h>
#include <vector>

#include "Containers/Vector.h"

static void DefaultBench(benchmark::State& state) {
  for (auto _ : state)
  {
  }
}

BENCHMARK(DefaultBench);

BENCHMARK_MAIN();