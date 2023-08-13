#pragma once

#include "Math.h"

#include "General/Numeric.h"
#include "General/Utility.h"
#include "functional"

namespace SSTD
{
  template<typename T, size_t Dim>
    requires(Dim > 0)
  struct Vec
  {
    template<size_t ... Indices>
    struct SwizzleProxy
    {
      SwizzleProxy(T* data) : ptr(data) {}

      SwizzleProxy(const SwizzleProxy&) = delete;
      SwizzleProxy(SwizzleProxy&&) = delete;

      SwizzleProxy& operator=(const SwizzleProxy&) = delete;
      SwizzleProxy& operator=(SwizzleProxy&&) = delete;

      template<size_t ... XIndices>
      SwizzleProxy<Indices...>& operator=(const SwizzleProxy<XIndices...>& other)
        requires(sizeof...(Indices) == sizeof...(XIndices))
      {
        const T other_values[]{ other.ptr[XIndices]... };
        size_t i = 0;
        ((ptr[Indices] = other_values[i++]), ...);

        return *this;
      }

      operator Vec<T, sizeof...(Indices)>() const { return Vec<T, sizeof...(Indices)>{ptr[Indices]...}; }

    private:
      T* ptr;
    };

  public:

    Vec() {}
    Vec(const T& value) noexcept { data = { value }; }

    template<typename ... Args>
      requires (sizeof...(Args) == Dim)
    Vec(Args&&... args) : data{ static_cast<T>(args)... } {}

    template<size_t OtherDim>
      requires (Dim <= OtherDim)
    Vec(const Vec<T, OtherDim>& other)
    {
      TMemCpy(data, other.data, Dim);
    }

    Vec(const T* arr) { TMemCpy(data, arr, Dim); }

    const T& At(size_t index) const { if (index < Dim) return this->operator[](index); }
    T& At(size_t index) { if (index < Dim) return this->operator[](index); }

    const T& operator[](size_t index) const { return data[index]; }
    T& operator[](size_t index) { return data[index]; }


    template<size_t Index>
    constexpr auto Swizzle() const requires (Dim >= Index)
    {
      return data[Index];
    }

    template<size_t ... Indices>
    constexpr auto Swizzle() const requires (sizeof...(Indices) > 1) && ((Dim >= Indices) && ...)
    {
      return SwizzleProxy<Indices...>{ data };
    }

#define VEC_SWIZZLE(op, ...)                                \
    constexpr auto op() const { return Swizzle<__VA_ARGS__>(); }  \

#define VEC_OPERATOR_ASSIGNMENT(op)             \
    constexpr Vec& operator op(const Vec& other)\
    {                                           \
      for (size_t i = 0; i < Dim; ++i)          \
        data[i] op other.data[i];               \
      return *this;                             \
    }                                           \
                                                \
    template<typename U>                        \
    constexpr Vec& operator op(const U& scalar) \
    {                                           \
      for (size_t i = 0; i < Dim; ++i)          \
        data[i] op static_cast<U>(scalar);      \
      return *this;                             \
    }                                           \

#define VEC_OPERATOR_UNARY(op)                  \
    constexpr Vec operator op()                 \
    {                                           \
      Vec out{};                                \
      for (size_t i = 0; i < Dim; ++i)          \
        out.data[i] = op data[i];               \
      return out;                               \
    }                                           \


#define VEC_OPERATOR_BINARY(op)                                   \
    constexpr Vec operator op(const Vec& other)                   \
    {                                                             \
      Vec out{};                                                  \
      for (size_t i = 0; i < Dim; ++i)                            \
        out.data[i] = data[i] op other.data[i];                   \
      return out;                                                 \
    }                                                             \
                                                                  \
    template<typename U>                                          \
    constexpr Vec operator op(const U& scalar)                    \
    {                                                             \
      Vec out{};                                                  \
      for (size_t i = 0; i < Dim; ++i)                            \
        out.data[i] = data[i] op static_cast<T>(scalar);          \
      return out;                                                 \
    }                                                             \

#define VEC_OPERATOR_COMPARISON(op)                               \
    constexpr Vec<bool,Dim> operator op(const Vec& other)         \
    {                                                             \
      Vec out{};                                                  \
      for (size_t i = 0; i < Dim; ++i)                            \
        out.data[i] = data[i] op other.data[i];                   \
      return out;                                                 \
    }                                                             \

#define VEC_OPERATOR_INCDEC(op)                 \
    constexpr Vec& operator op()                \
    {                                           \
      for (size_t i = 0; i < Dim; ++i)          \
        op data[i];                             \
      return *this;                             \
    }                                           \
    constexpr Vec operator op(int)              \
    {                                           \
      Vec out{};                                \
      this->operator op();                      \
      return out;                               \
    }                                           \
    
    VEC_SWIZZLE(x, 0);
    VEC_SWIZZLE(y, 1);
    VEC_SWIZZLE(z, 2);
    VEC_SWIZZLE(w, 3);

    VEC_SWIZZLE(xy, 0, 1);
    VEC_SWIZZLE(yx, 1, 0);

    VEC_SWIZZLE(xx, 0, 0);
    VEC_SWIZZLE(yy, 1, 1);
    VEC_SWIZZLE(zz, 2, 2);

    VEC_SWIZZLE(xyz, 0, 1, 2);
    VEC_SWIZZLE(xzy, 0, 2, 1);
    VEC_SWIZZLE(yxz, 1, 0, 2);
    VEC_SWIZZLE(yzx, 1, 2, 0);
    VEC_SWIZZLE(zxy, 2, 0, 1);
    VEC_SWIZZLE(zyx, 2, 1, 0);

    VEC_SWIZZLE(xxx, 0, 0, 0);
    VEC_SWIZZLE(yyy, 1, 1, 1);
    VEC_SWIZZLE(zzz, 2, 2, 2);

    VEC_OPERATOR_ASSIGNMENT(+= );
    VEC_OPERATOR_ASSIGNMENT(-= );
    VEC_OPERATOR_ASSIGNMENT(*= );
    VEC_OPERATOR_ASSIGNMENT(/= );

    VEC_OPERATOR_UNARY(!);
    VEC_OPERATOR_UNARY(~);
    VEC_OPERATOR_UNARY(-);

    VEC_OPERATOR_COMPARISON(== );
    VEC_OPERATOR_COMPARISON(!= );
    VEC_OPERATOR_COMPARISON(>= );
    VEC_OPERATOR_COMPARISON(<= );
    VEC_OPERATOR_COMPARISON(< );
    VEC_OPERATOR_COMPARISON(> );

    VEC_OPERATOR_BINARY(+);
    VEC_OPERATOR_BINARY(-);
    VEC_OPERATOR_BINARY(*);
    VEC_OPERATOR_BINARY(/ );

    VEC_OPERATOR_INCDEC(++);
    VEC_OPERATOR_INCDEC(--);

    constexpr T Sum() const
    {
      T out{ 0 };
      for (size_t i = 0; i < Dim; ++i)
        out += data[i];
    }

    constexpr T Prod() const
    {
      T out{ 1 };
      for (size_t i = 0; i < Dim; ++i)
        out *= data[i];
    }

    constexpr T Dot(const Vec& other)
    {
      T out{};
      for (size_t i = 0; i < Dim; i++)
        out += data[i] * other.data[i];
      return out;
    }

    constexpr auto Cross(const Vec& other)
      requires (Dim < 4)
    {
      if constexpr (Dim == 2)
        return x() * other.y() - other.x() * y();
      else if constexpr (Dim == 3)
        return {
          (y() * other.z() - z() * other.y()),
          (z() * other.x() - x() * other.z()),
          (x() * other.y() - y() * other.x())
      };
    }

    constexpr T Length() const { return Math::Sqrt(SqrLength()); }
    constexpr T SqrLength() const
    {
      T out{ 0 };
      for (size_t i = 0; i < Dim; ++i)
        out += (data[i] * data[i]);
    }

    Vec& Normalize() const { return ((*this) / Length()); }

    constexpr size_t Size() { return Dim; }

    T data[Dim]{};
  };

  template<typename T>
  using Vec2 = Vec<T, 2>;

  template<typename T>
  using Vec3 = Vec<T, 3>;

  template<typename T>
  using Vec4 = Vec<T, 4>;

  template<typename T>
  constexpr T Cross(const Vec2<T>& a, const Vec2<T>& b)
  {
    return a.x() * b.y() - a.x() * b.y();
  }
}