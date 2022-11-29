#pragma once

#include "General/Pattern.h"

namespace SSTD
{
  //Locks provide some kind of auto mutex, it uses it lifetime for the lock and unlock

  template<typename T>
  class Lock : public NonCopyable
  {
  public:
    Lock(T& mutex)
      :m_Lock( __builtin_addressof(mutex)), m_Owned(false)
    {
      m_Lock->Lock();
      m_Owned = true;
    }
    ~Lock()
    {
      Unlock();
    }

    void Unlock()
    {
      m_Lock->Unlock();
      m_Owned = false;
    }
    T& GetLock()
    {
      return *m_Lock;
    }
  private:
    T* m_Lock;
    bool m_Owned;
  };

  template<typename T>
  class ConditionalLock : public NonCopyable
  {
  public:
    ConditionalLock(T &lock, bool condition)
      :m_Lock(lock), m_Condition(condition)
    {
      if (m_Condition)
        m_Lock.Lock();
    }
    ~ConditionalLock()
    {
      if (m_Condition)
        m_Lock.Unlock();
    }
    void Unlock()
    {
      if (m_Condition)
        m_Lock.Unlock();
    }
  private:
    T& m_Lock;
    bool m_Condition;
  };
}
