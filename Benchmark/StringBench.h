#include <benchmark/benchmark.h>

#include <string>
#include "Containers/String.h"

#define BENCH_STRING10 "HelloWorld"
#define BENCH_STRING31 "v7YJtWfzGJpK1NcHx0LlD9BmEOaS6yR"
#define BENCH_STRING64 "v7YJtWfzGJpK1NcHx0LlD9BmEOaS6yRv7YJtWfzGJpK1NcHx0LlD9BmEOaS6yRal"

namespace BenchString
{
  static constexpr const char* bench_data[] = {
    "cg",
    "zmcw",
    "ozawqxwd",
    "itynjrfzfojmlnwp",
    "gwikmpgmpvegybjdtrujjcyobbsrjpgo",
    "fjbqbacqnoeegmhperbfiygxcocgewyqxebbfyacbomvdfvbfitkwfyfuplnncov",
    "tmmwjullgkykopcqfmolfjrqswqnxuxdgkpchrcnuuwufnrppupntcanagkerclbokcnaxrdzwyanpuxwfowyghburcrquljogucmndmytuxoqgklgypakpjyclxqpdm",
    "eyucbjceusjtnpxpunkkntfhpnuhkrxmmrtkcnlgyesvaiylludaolzaztwwtudzygpwpeuygrgbsmhcqdvxwqpfniwupbqgisgwfdguamffvfqybcdtkioplajxalwumcfzdjsgpznhahelyxaaxqrwhtzocixlasdtxvpkaylqfvciinklcgwdkjctigpdmcipllqyuizzhmbqxywrrgesevgsihmtkrbvxbmdjqindqhitmjonopmgfsaysxu",
    "piyhhsffxygyjuvvhqfgwiiwnlstgdgbgjmkrewpapekvezfvqgxzrgizmdhopebzxxnmjlbefunjyaludyfbtoytqlguquhncfageflpzzdyypknyuugdpwfeunmvmwgaoijougpudvqfsftiurpfazjlvfcwlfoipexedkzuwqmfoeijivqnavatfegqmnhckwgtlyepybddcwohckvivlzbaejwsecozcckpmqdunquzpncoinkyumqkaubmxejadiobmrdwcxwhnwfweebnzuvzktjntlrbdaujhyxecmnqpksedabnnixsoodbkrzqscxrarbyhvhytkbtieceluorqbjbvmgaheujphnbojxtqtrzhfqbnowqwawbcgemhpgehxmrmnatdsqsjletpjnqeqggjrqgpcjbdhobepbhdvrljuznuplquudnnpecdoiitrtxdxlsswapwrkqahgvkcalsxoljtkauuaymveiztxhnwldbaqkqahin"
  };

  static constexpr size_t bench_size = (sizeof(bench_data) / sizeof(const char*)) -1;

  static void ConstructorEmpty(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s{};
      benchmark::DoNotOptimize(s);
    }
  }

  static void Constructor(benchmark::State& state)
  {
    for (auto _ : state)
    {
      SSTD::String s(bench_data[state.range(0)]);
      benchmark::DoNotOptimize(s);
    }
  }

  static void CopyConstructor(benchmark::State& state)
  {
    SSTD::String s(bench_data[state.range(0)]);
    benchmark::DoNotOptimize(s);
    for (auto _ : state)
    {
      SSTD::String s2(s);
      benchmark::DoNotOptimize(s2);
    }
  }

  static void MoveConstructor(benchmark::State& state)
  {
    SSTD::String s(bench_data[state.range(0)]);
    benchmark::DoNotOptimize(s);
    for (auto _ : state)
    {
      SSTD::String s2(Move(s));
      benchmark::DoNotOptimize(s2);
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

    static void Constructor(benchmark::State& state)
    {
      for (auto _ : state)
      {
        std::string s(bench_data[state.range(0)]);
        benchmark::DoNotOptimize(s);
      }
    }

    static void CopyConstructor(benchmark::State& state)
    {
      std::string s(bench_data[state.range(0)]);
      benchmark::DoNotOptimize(s);

      for (auto _ : state)
      {
        std::string s2(s);
        benchmark::DoNotOptimize(s2);
      }
    }

    static void MoveConstructor(benchmark::State& state)
    {
      std::string s(bench_data[state.range(0)]);
      benchmark::DoNotOptimize(s);

      for (auto _ : state)
      {
        std::string s2(std::move(s));
        benchmark::DoNotOptimize(s2);
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

BENCHMARK(BenchString::Constructor)->DenseRange(0, BenchString::bench_size);
BENCHMARK(BenchString::STD::Constructor)->DenseRange(0, BenchString::bench_size);

BENCHMARK(BenchString::CopyConstructor)->DenseRange(0, BenchString::bench_size);
BENCHMARK(BenchString::STD::CopyConstructor)->DenseRange(0, BenchString::bench_size);

BENCHMARK(BenchString::MoveConstructor)->DenseRange(0, BenchString::bench_size);
BENCHMARK(BenchString::STD::MoveConstructor)->DenseRange(0, BenchString::bench_size);

BENCHMARK(BenchString::Reserve)->Range(1, 1 << 18);
BENCHMARK(BenchString::STD::Reserve)->Range(1, 1 << 18);