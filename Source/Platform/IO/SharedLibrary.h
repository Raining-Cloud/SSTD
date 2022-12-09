#pragma once

#include "General/Numeric.h"
#include "General/Allocator.h"

#include "Containers/String.h"
#include "Containers/Vector.h"

#include "Platform/DefinePlatform.h"

/*
Some general thoughts on this I might forget about:
- We need to statically type the dll return value / args.  this is due to security and it should give some speed/memory improvements. We know thoose anyways and won't support some fancy dll modding
- We use a handle for every function we create, the handle should directly map to the memory location the ptr is stored.
*/

namespace SSTD
{
  struct SharedLibaryFunctionHandle
  {
    static const uint32 invalid = NumericLimit<uint32>::max;

    SharedLibaryFunctionHandle(uint32 val) : id(val) {}
    uint32 id = NumericLimit<uint32>::max;
  };

  //template<template<typename> typename A = Allocator>
  class SharedLibrary
  {
    struct ICallable
    {
      virtual void ReloadFrom(void* handle) = 0;
      const String func_name = "Invalid";
    };

    template<typename ReturnValue, typename ... Args>
    struct Callable : public ICallable
    {
      typedef ReturnValue(*Apply)(Args...);

      Callable(const String& funcname, Apply f) : func_name(func_name), func(f) {}
      virtual void ReloadFrom(void* handle) override final { func = LoadFunction(func_name, handle); }
      Apply func = nullptr;
    };

  public:
    SharedLibrary(const String& path);
    SharedLibrary(const SharedLibrary& other);
    SharedLibrary(SharedLibrary&& other) noexcept;
    ~SharedLibrary();

    SharedLibrary& operator=(const SharedLibrary& other);
    SharedLibrary& operator=(SharedLibrary&& other) noexcept;

    template<typename ReturnValue, typename ... Args>
    ReturnValue operator()(const SharedLibaryFunctionHandle& func_handle, Args&&... args) { return CallFunction<ReturnValue, Args>(func_handle, Forward<Args>(args)...); }

    void Load();
    void TryHotReload();
    void Free();

    template<typename ReturnValue, typename ... Args>
    SharedLibaryFunctionHandle LoadFunction(const String& funcname)
    {
      m_FuncPtrs.EmplaceBack<Callable<ReturnValue, Args>>(funcname, LoadFunction(funcname, m_Handle));
      return { m_FuncPtrs.Size() - 1 };
    }

    template<typename ReturnValue, typename ... Args>
    ReturnValue SafeCall(const SharedLibaryFunctionHandle& func_handle, Args&&... args)
    {
      if(m_FuncPtrs.Size() < func_handle.id)
        return dynamic_cast<Callable<ReturnValue, Args>*>(m_FuncPtrs[func_handle])->func(Forward<Args>(args)...);
      throw;
    }

    template<typename ReturnValue, typename ... Args>
    ReturnValue CallFunction(const SharedLibaryFunctionHandle& func_handle, Args&&... args)
    {
      return dynamic_cast<Callable<ReturnValue, Args>*>(m_FuncPtrs[func_handle.id])->func(Forward<Args>(args)...);
    }

  private:
    static void* LoadFunction(const String& funcname, void* handle);

    String m_FilePath;
    Vector<ICallable*> m_FuncPtrs;
#ifdef PLATFORM_WIN64
    void* m_Handle = nullptr;
    uint64 m_LastWriteTime;
#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
  };
}