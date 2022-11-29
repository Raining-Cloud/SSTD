#pragma once

#include "General/Concepts.h"
#include "General/Numeric.h"

namespace SSTD
{

  template<typename T>
  class HandleMap
  {
  public:
    using HandleType = uint32;
    explicit HandleMap() {}
    HandleMap(const HandleMap& other) {}
    HandleMap(HandleMap&& other) {}
    ~HandleMap() {}

    bool Validate(HandleType handle) {}
    
  private:

  };
}