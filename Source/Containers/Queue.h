#include <queue>

namespace SSTD
{
  //yet another wrapper

  template<typename T>
  using Queue = std::queue<T>;

  template<typename T>
  using Dequeue = std::deque<T>;
}

