#pragma once

#define VectorBench 1
#define StringBench 0

#if VectorBench
  #include "VectorBench.h"
#endif // VectorBench

#if StringBench
  #include "StringBench.h"
#endif // StringBench

