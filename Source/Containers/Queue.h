#include <queue>

namespace SSTD
{
  //I am not smart enough to implement a efficient data-strcuture on my own yet

  template<typename T>
  using Queue = std::queue<T>;

  template<typename T>
  using Dequeue = std::deque<T>;
}