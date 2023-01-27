#pragma once
#include "General/Utility.h"
#include "General/Meta.h"

#include "General/Exception.h"

namespace SSTD
{
  template<typename T>
  class Function;

  template<typename ReturnValue, typename ... Args>
  class Function<ReturnValue(Args...)>
  {
    struct ICallable
    {
      virtual void Delete() = 0;
      virtual ICallable* Copy(void* dst) = 0;
      virtual ICallable* Move(void* dst) = 0;
      virtual ReturnValue Invoke(Args... args) = 0;
    };

    static const uint32 ReservedStackSize = 64 - sizeof(ICallable);

    template<typename T>
    struct CallableObject : public ICallable
    {
      static const bool HasAlloc = sizeof(T) > ReservedStackSize;

      explicit CallableObject(T&& val) : obj(Forward<T>(val)) {}

      void Delete() override final
      {
        if (HasAlloc)
          delete this;
      };
      ICallable* Copy(void* dst) override final
      {
        if constexpr (HasAlloc)
          return new CallableObject(obj);
        else
          return new (dst) CallableObject(obj);
      };
      ICallable* Move(void* dst) override final
      {
        if constexpr (HasAlloc)
          return this;
        else
          return new (dst) CallableObject(obj);
      };
      ReturnValue Invoke(Args... args) override final { return obj(Forward<Args>(args)...); };

      T obj;
    };

    template<ReturnValue(*Func)(Args...)>
    struct CallableGlobal : public ICallable
    {
      static const bool HasAlloc = false;
      CallableGlobal() {}

      void Delete() override final {};
      ICallable* Copy(void* dst) override final { return new (dst) CallableGlobal(); };
      ICallable* Move(void* dst) override final { return this; };
      ReturnValue Invoke(Args... args) override final { return Func(Forward<Args>(args)...); };
    };

    template<typename T, ReturnValue(T::* Func)(Args...)>
    struct CallableMember : public ICallable
    {
      static const bool HasAlloc = false;

      CallableMember(T* object) : obj(object) {}
      void Delete() override final {};
      ICallable* Copy(void* dst) override final { return new (dst) CallableMember(obj); };
      ICallable* Move(void* dst) override final { return this; };
      ReturnValue Invoke(Args... args) override final { return (obj->*Func)(Forward<Args>(args)...); };
      T* obj;
    };

    template<typename T, const ReturnValue(T::* Func)(Args...)>
    struct CallableConstMember : public ICallable
    {
      static const bool HasAlloc = false;

      CallableConstMember(const T& object) : obj(const_cast<T*>(&object)) {}
      void Delete() override final {};
      ICallable* Copy(void* dst) override final { return new (dst) CallableConstMember(*obj); };
      ICallable* Move(void* dst) override final { return this; };
      ReturnValue Invoke(Args... args) override final { return (obj->*Func)(Forward<Args>(args)...); };
      T* obj;
    };

  public:
    explicit Function() : m_Callable(nullptr) {}

    Function(const Function& other)
      : m_Callable(other.m_Callable->Copy(&m_Buffer))
    {}

    Function(Function&& other) noexcept
      : m_Callable(other.m_Callable->Move(&m_Buffer))
    {
      other.m_Callable = nullptr;
    }

    ~Function() { Clear(); }

    Function& operator=(const Function& other)
    {
      Clear();
      m_Callable = other.m_Callable->Copy(&m_Buffer);
      return *this;
    }
    Function& operator=(Function&& other) noexcept 
    {
      Clear();
      m_Callable = other.m_Callable->Move(&m_Buffer);
      other.m_Callable = nullptr;
      return *this;
    }

    ReturnValue operator()(Args... args) { return m_Callable->Invoke(Forward<Args>(args)...); }
    ReturnValue Invoke(Args&&... args) 
    { 
      if (m_Callable)
        return m_Callable->Invoke(Forward<Args>(args)...); 

      throw Exception();
    }

    template<typename T>
    void Bind(T&& val)
    {
      Clear();
      Construct<CallableObject<T>>(Forward<T>(val));
    }

    template<ReturnValue(*Func)(Args...)>
    void Bind() 
    {
      Clear();
      Construct<CallableGlobal<Func>>();
    }

    template<typename T, ReturnValue(T::* Func)(Args...)>
    void Bind(T* obj) 
    {
      Clear();
      Construct<CallableMember<T, Func>>(obj);
    }

    template<typename T, const ReturnValue(T::* Func)(Args...)>
    void Bind(const T& obj)
    {
      Clear();
      Construct<CallableConstMember<T, Func>>(obj);
    }

    template<typename T>
    static constexpr Function Create(T&& val)
    {
      Function f;
      f.Construct<CallableObject<T>>(Forward<T>(val));
      return f;
    }

    template<ReturnValue(*Func)(Args...)>
    static constexpr Function Create()
    {
      Function f;
      f.Construct< CallableGlobal<Func>>();
      return f;
    }

    template<typename T, ReturnValue(T::* Func)(Args...)>
    static constexpr Function Create(T* obj)
    {
      Function f;
      f.Construct<CallableMember<T, Func>>(obj);
      return f;
    }

    template<typename T, const ReturnValue(T::* Func)(Args...)>
    static constexpr Function Create(const T& obj)
    {
      Function f;
      f.Construct<CallableConstMember<T, Func>>(obj);
      return f;
    }

    void Clear()
    {
      if (m_Callable)
      {
        m_Callable->Delete();
        m_Callable = nullptr;
      }
    }

  private:

    template<typename T, typename ... Args2>
    constexpr void Construct(Args2&&... args)
    {
      if constexpr (T::HasAlloc)
        m_Callable = new T(Forward<Args2>(args)...);
      else
        m_Callable = new (&m_Buffer) T(Forward<Args2>(args)...);
    }

    uint8 m_Buffer[ReservedStackSize]{};
    ICallable* m_Callable = nullptr;
  };
}