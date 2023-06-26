#include <benchmark/benchmark.h>

#include <vector>
#include "Containers/Vector.h"

namespace BenchVector
{


  static void ConstructorEmpty(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::Vector<int> v;
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

  static void ConstructorPushBack(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::Vector<int> v{};
      v.PushBack(16);
      benchmark::DoNotOptimize(v);
    }
  }

  static void Reserve(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::Vector<int> v;
      v.Reserve(state.range(0));
      benchmark::DoNotOptimize(v);
    }
  }


  namespace STD
  {
    static void ConstructorEmpty(benchmark::State& state)
    {
      for (auto _ : state)
      {
        std::vector<int> v;
        benchmark::DoNotOptimize(v);
      }
    }

    static void ConstructorReserve(benchmark::State& state)
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
        std::vector<int> v{};
        v.push_back(16);
        benchmark::DoNotOptimize(v);
      }
    }

    static void Reserve(benchmark::State& state)
    {
      for (auto _ : state)
      {
        std::vector<int> v;
        v.reserve(state.range(0));
        benchmark::DoNotOptimize(v);
      }
    }
  }
}

BENCHMARK(BenchVector::ConstructorEmpty);
BENCHMARK(BenchVector::STD::ConstructorEmpty);

BENCHMARK(BenchVector::ConstructorReserve);
BENCHMARK(BenchVector::STD::ConstructorReserve);

BENCHMARK(BenchVector::ConstructorPushBack);
BENCHMARK(BenchVector::STD::ConstructorPushBack);

BENCHMARK(BenchVector::Reserve)->Range(1, 1<<18);
BENCHMARK(BenchVector::STD::Reserve)->Range(1, 1 << 18);