#pragma once

namespace SSTD
{
  using int8 = signed char;
  using int16 = short;
  using int32 = int;
  using int64 = long long;

  using uint8 = unsigned char;
  using uint16 = unsigned short;
  using uint32 = unsigned int;
  using uint64 = unsigned long long;

  using double32 = double;
  using double64 = long double;

  using size_t = decltype(sizeof(void*));

  template<typename T>
  struct IsNumeric { static constexpr bool valid = false; };

  //Int
  template<>
  struct IsNumeric<int8> { static constexpr bool valid = true; };
  template<>
  struct IsNumeric<int16> { static constexpr bool valid = true; };
  template<>
  struct IsNumeric<int32> { static constexpr bool valid = true; };
  template<>
  struct IsNumeric<int64> { static constexpr bool valid = true; };

  //uInt
  template<>
  struct IsNumeric<uint8> { static constexpr bool valid = true; };
  template<>
  struct IsNumeric<uint16> { static constexpr bool valid = true; };
  template<>
  struct IsNumeric<uint32> { static constexpr bool valid = true; };
  template<>
  struct IsNumeric<uint64> { static constexpr bool valid = true; };

 //Float + Double
  template<>
  struct IsNumeric<float> { static constexpr bool valid = true; };

  template<>
  struct IsNumeric<double> { static constexpr bool valid = true; };

  template<>
  struct IsNumeric<double64> { static constexpr bool valid = true; };

  template<typename T>
  struct NumericLimit { static constexpr uint32 value = 0; };

  template<typename T>
  struct NumericLimit;

  template<>
  struct NumericLimit<int8>
  {
    static constexpr int8 max = 0x7f;
    static constexpr int8 min = 0x80;
  };
  
  template<>
  struct NumericLimit<int16>
  {
    static constexpr int16 max = 0x7f'ff;
    static constexpr int16 min = 0x80'00;
  };

  template<>
  struct NumericLimit<int32>
  {
    static constexpr int32 max = 0x7f'ff'ff'ff;
    static constexpr int32 min = 0x80'00'00'00;
  };

  template<>
  struct NumericLimit<int64>
  {
    static constexpr int64 max = 0x7f'ff'ff'ff'ff'ff'ff'ff;
    static constexpr int64 min = 0x80'00'00'00'00'00'00'00;
  };

  template<>
  struct NumericLimit<uint8>
  {
    static constexpr uint8 max = -1;
    static constexpr uint8 min = 0;
  };
  
  template<>
  struct NumericLimit<uint16>
  {
    static constexpr uint16 max = -1;
    static constexpr uint16 min = 0;
  };

  template<>
  struct NumericLimit<uint32>
  {
    static constexpr uint32 max = -1;
    static constexpr uint32 min = 0;
  };

  template<>
  struct NumericLimit<uint64>
  {
    static constexpr uint64 max = -1;
    static constexpr uint64 min = 0;
  };

  template<size_t>
  struct NumericTypeFromSize;

  template<>
  struct NumericTypeFromSize<1>
  {
    using Signed = int8;
    using Unsigned = uint8;
  };

  template<>
  struct NumericTypeFromSize<2>
  {
    using Signed = int16;
    using Unsigned = uint16;
  };

  template<>
  struct NumericTypeFromSize<4>
  {
    using Signed = int32;
    using Unsigned = uint32;
  };

  template<>
  struct NumericTypeFromSize<8>
  {
    using Signed = int64;
    using Unsigned = uint64;
  };

}