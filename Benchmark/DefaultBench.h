#include <benchmark/benchmark.h>

namespace SSTDBench
{
  static void DefaultBench(benchmark::State& state)
  {
    for (auto _ : state)
    {
    }
  }
}

BENCHMARK(SSTDBench::DefaultBench);