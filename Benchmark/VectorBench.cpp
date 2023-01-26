#include <benchmark/benchmark.h>

#include <vector>
#include "Containers/Vector.h"

namespace VectorBench
{
  static void ConstructorEmpty(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::Vector<int> v{};
      benchmark::DoNotOptimize(v);
    }
  }

  static void STDConstructorEmpty(benchmark::State& state)
  {
    for (auto _ : state)
    {
      std::vector<int> v{};
      benchmark::DoNotOptimize(v);
    }
  }

  static void ConstructorReserve(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::Vector<int> v{16};
      benchmark::DoNotOptimize(v);
    }
  }

  static void STDConstructorReserve(benchmark::State& state)
  {
    for (auto _ : state)
    {
      std::vector<int> v{};
      v.reserve(16);
      benchmark::DoNotOptimize(v);
    }
  }

  static void ConstructorPushBack(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::Vector<int> v{};
      v.PushBack(16);
      benchmark::DoNotOptimize(v);
    }
  }

  static void STDConstructorPushBack(benchmark::State& state)
  {
    for (auto _ : state)
    {
      std::vector<int> v{};
      v.push_back(16);
      benchmark::DoNotOptimize(v);
    }
  }
}

BENCHMARK(VectorBench::ConstructorEmpty);
BENCHMARK(VectorBench::STDConstructorEmpty);

BENCHMARK(VectorBench::ConstructorReserve);
BENCHMARK(VectorBench::STDConstructorReserve);

BENCHMARK(VectorBench::ConstructorPushBack);
BENCHMARK(VectorBench::STDConstructorPushBack);