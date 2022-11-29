#include "SharedLibrary.h"

#include "Platform/IncludePlatform.h"
#include "General/Memory.h"

namespace SSTD
{
#ifdef PLATFORM_WIN64

  SharedLibrary::SharedLibrary(const String& path)
    : m_FilePath(path)
  {
  }

  SharedLibrary::SharedLibrary(const SharedLibrary& other)
  {
  }

  SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept
  {
  }

  SharedLibrary::~SharedLibrary()
  {
    Free();
  }

  SharedLibrary& SharedLibrary::operator=(const SharedLibrary& other)
  {
    return *this;
  }

  SharedLibrary& SharedLibrary::operator=(SharedLibrary&& other) noexcept
  {
    return *this;
  }

  void SharedLibrary::Load()
  {
    m_Handle = LoadLibraryA(m_FilePath.CStr());
    if(m_Handle)
      GetFileTime(m_Handle, nullptr, nullptr, reinterpret_cast<FILETIME*>(&m_LastWriteTime));
  }

  void SharedLibrary::TryHotReload()
  {
    FILETIME time;
    GetFileTime(m_Handle, nullptr, nullptr, &time);
    if (CompareFileTime(reinterpret_cast<FILETIME*>(&m_LastWriteTime), &time))
    {
      m_Handle = LoadLibraryA(m_FilePath.CStr());
      for (auto& f : m_FuncPtrs)
        f->ReloadFrom(m_Handle); //this way we keep the same-handles :D
    }
  }
  void SharedLibrary::Free()
  {
    FreeLibrary(static_cast<HMODULE>(m_Handle));
  }

  void* SharedLibrary::LoadFunction(const String& funcname, void* handle)
  {
    return GetProcAddress(static_cast<HMODULE>(handle), funcname.CStr());
  }

#elif  PLATFORM_WIN32
#elif  PLATFORM_IPHONE_SIM
#elif  PLATFORM_IPHONE
#elif  PLATFORM_MAC
#elif  PLATFORM_LINUX
#endif
}