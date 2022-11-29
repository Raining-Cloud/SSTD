#pragma once

#include "Containers/String.h"

namespace SSTD
{
  class File
  {
  public:
    File(const String& path);
    File(const File& other);
    File(File&& other) noexcept;
    ~File();

    String GetPath() { return m_Path; }
    String GetDirectory() { throw; }
  private:
    String m_Path;
  };
}