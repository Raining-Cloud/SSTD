#include <benchmark/benchmark.h>

#include <string>
#include "Containers/String.h"

#define BENCH_STRING10 "HelloWorld"
#define BENCH_STRING31 "v7YJtWfzGJpK1NcHx0LlD9BmEOaS6yR"
#define BENCH_STRING64 "v7YJtWfzGJpK1NcHx0LlD9BmEOaS6yRv7YJtWfzGJpK1NcHx0LlD9BmEOaS6yRal"

namespace BenchString
{
  static void ConstructorEmpty(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s{};
      benchmark::DoNotOptimize(s);
    }
  }

  static void Constructor10(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s(BENCH_STRING10);
      benchmark::DoNotOptimize(s);
    }
  }

  static void Constructor31(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s(BENCH_STRING31);
      benchmark::DoNotOptimize(s);
    }
  }

  static void CopyConstructor31(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s(BENCH_STRING31);
      SSTD::String s2(s);
      benchmark::DoNotOptimize(s);
      benchmark::DoNotOptimize(s2);
    }
  }

  static void Constructor64(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s(BENCH_STRING64);
      benchmark::DoNotOptimize(s);
    }
  }

  static void Reserve(benchmark::State& state)
  {
    for (auto _ : state) {
      SSTD::String str;
      str.Reserve(state.range(0));  // Reserving the specified capacity
    }
  }

  namespace STD
  {
    static void ConstructorEmpty(benchmark::State& state)
    {
      for (auto _ : state)
      {
        std::string s{};
        benchmark::DoNotOptimize(s);
      }
    }

    static void Constructor10(benchmark::State& state)
    {
      for (auto _ : state)
      {
        std::string s(BENCH_STRING10);
        benchmark::DoNotOptimize(s);
      }
    }

    static void Constructor31(benchmark::State& state)
    {
      for (auto _ : state)
      {
        SSTD::String s(BENCH_STRING31);
        benchmark::DoNotOptimize(s);
      }
    }

    static void CopyConstructor31(benchmark::State& state)
    {
      for (auto _ : state)
      {
        SSTD::String s(BENCH_STRING31);
        SSTD::String s2(s);
        benchmark::DoNotOptimize(s);
        benchmark::DoNotOptimize(s2);
      }
    }

    static void Constructor64(benchmark::State& state)
    {
      for (auto _ : state)
      {
        std::string s(BENCH_STRING64);
        benchmark::DoNotOptimize(s);
      }
    }

    static void Reserve(benchmark::State& state)
    {
      for (auto _ : state) {
        std::string str;
        str.reserve(state.range(0));  // Reserving the specified capacity
      }
    }
  }
}

BENCHMARK(BenchString::ConstructorEmpty);
BENCHMARK(BenchString::STD::ConstructorEmpty);

BENCHMARK(BenchString::Constructor10);
BENCHMARK(BenchString::STD::Constructor10);

BENCHMARK(BenchString::Constructor31);
BENCHMARK(BenchString::STD::Constructor31);

BENCHMARK(BenchString::Constructor64);
BENCHMARK(BenchString::STD::Constructor64);

BENCHMARK(BenchString::CopyConstructor31);
BENCHMARK(BenchString::STD::CopyConstructor31);

BENCHMARK(BenchString::Reserve)->Range(1, 1 << 18);
BENCHMARK(BenchString::STD::Reserve)->Range(1, 1 << 18);