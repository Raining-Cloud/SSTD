#include "File.h"

namespace SSTD
{
  File::File(const String& path)
    : m_Path(path)
  {
  }
  File::File(const File& other)
    : m_Path(other.m_Path)
  {
  }

  File::File(File&& other) noexcept
    : m_Path(other.m_Path)
  {
  }
  File::~File()
  {
  }
}