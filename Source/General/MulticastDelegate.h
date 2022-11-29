#pragma once
#include <list>
#include "Delegate.h"

template<typename...Args>
class MulticastDelegate
{
private:
  using MulticastInternalFunction = Delegate<void(Args...)>;
public:
  MulticastDelegate() {}
  ~MulticastDelegate() {}

  void Add(const MulticastInternalFunction& del)
  {
    m_Delegates.push_back(del);
  }

  void Invoke(Args... args) const
  {
    for (const auto& d : m_Delegates)
      d.Invoke(); //TODO: forward the args!
  }

  void InvokeIfBound(Args... args) const
  {
    for (const auto& d : m_Delegates)
      d.InvokeIfBound(); //TODO: forward the args!
  }

  void operator()(Args... args) const
  {
    Invoke(args...);
  }
private:
  std::list<MulticastInternalFunction> m_Delegates;
};