#pragma once

#include "Math.h"

#include "General/Numeric.h"

namespace SSTD
{
  template<typename T, size_t Dim>
    requires(Dim > 0)
  struct Vec
  {
  public:
    Vec() noexcept {}
    explicit Vec(const T& value) noexcept : m_Buffer{ value } {}

    template<typename ... Args>
      requires (sizeof...(Args) == Dim)
    Vec(Args&&... args) : m_Buffer{ static_cast<T>(args)... } {}

    template<size_t OtherDim>
      requires IsLessEqual<Dim, OtherDim>
    Vec(const Vec<T, OtherDim>& other) { TMemCpy(m_Buffer, other.Data(), Dim); }
    

#define VEC_ACCESSOR_CHAR(c, idx)                                       \
    T& c () requires(Dim > idx) { return m_Buffer[idx]; }               \
    const T& c () const requires(Dim > idx) { return m_Buffer[idx]; }   \

    VEC_ACCESSOR_CHAR(x, 0);
    VEC_ACCESSOR_CHAR(y, 1);
    VEC_ACCESSOR_CHAR(z, 2);
    VEC_ACCESSOR_CHAR(w, 3);

    const T& At(size_t index) const { if (index < Dim) return this->operator[](index); }
    T& At(size_t index) { if (index < Dim) return this->operator[](index); }
    const T& operator[](size_t index) const { return m_Buffer[index]; }
    T& operator[](size_t index) { return m_Buffer[index]; }


#define VEC_OPERATOR_ASSIGNMENT(op)             \
    constexpr Vec& operator op(const Vec& other)\
    {                                           \
      for (size_t i = 0; i < Dim; ++i)          \
        m_Buffer[i] op other.m_Buffer[i];       \
      return *this;                             \
    }                                           \
                                                \
    template<typename U>                        \
    constexpr Vec& operator op(const U& scalar) \
    {                                           \
      for (size_t i = 0; i < Dim; ++i)          \
        m_Buffer[i] op static_cast<U>(scalar);  \
      return *this;                             \
    }                                           \

#define VEC_OPERATOR_UNARY(op)                  \
    constexpr Vec operator op()                 \
    {                                           \
      Vec out{};                                \
      for (size_t i = 0; i < Dim; ++i)          \
        out.m_Buffer[i] = op m_Buffer[i];       \
      return out;                               \
    }                                           \


#define VEC_OPERATOR_BINARY(op)                                   \
    constexpr Vec operator op(const Vec& other)                   \
    {                                                             \
      Vec out{};                                                  \
      for (size_t i = 0; i < Dim; ++i)                            \
        out.m_Buffer[i] = m_Buffer[i] op other.m_Buffer[i];       \
      return out;                                                 \
    }                                                             \
                                                                  \
    template<typename U>                                          \
    constexpr Vec operator op(const U& scalar)                    \
    {                                                             \
      Vec out{};                                                  \
      for (size_t i = 0; i < Dim; ++i)                            \
        out.m_Buffer[i] = m_Buffer[i] op static_cast<T>(scalar);  \
      return out;                                                 \
    }                                                             \

#define VEC_OPERATOR_COMPARISON(op)                               \
    constexpr Vec<bool,Dim> operator op(const Vec& other)         \
    {                                                             \
      Vec out{};                                                  \
      for (size_t i = 0; i < Dim; ++i)                            \
        out.m_Buffer[i] = m_Buffer[i] op other.m_Buffer[i];       \
      return out;                                                 \
    }                                                             \

#define VEC_OPERATOR_INCDEC(op)                 \
    constexpr Vec& operator op()                \
    {                                           \
      for (size_t i = 0; i < Dim; ++i)          \
        op m_Buffer[i];                         \
      return *this;                             \
    }                                           \
    constexpr Vec operator op(int)              \
    {                                           \
      Vec out{};                                \
      this->operator op();                      \
      return out;                               \
    }                                           \

    VEC_OPERATOR_ASSIGNMENT(+= );
    VEC_OPERATOR_ASSIGNMENT(-= );
    VEC_OPERATOR_ASSIGNMENT(*= );
    VEC_OPERATOR_ASSIGNMENT(/= );

    VEC_OPERATOR_UNARY(!);
    VEC_OPERATOR_UNARY(~);
    VEC_OPERATOR_UNARY(-);

    VEC_OPERATOR_COMPARISON(==);
    VEC_OPERATOR_COMPARISON(!=);
    VEC_OPERATOR_COMPARISON(>=);
    VEC_OPERATOR_COMPARISON(<=);
    VEC_OPERATOR_COMPARISON(<);
    VEC_OPERATOR_COMPARISON(>);

    VEC_OPERATOR_BINARY(+);
    VEC_OPERATOR_BINARY(-);
    VEC_OPERATOR_BINARY(*);
    VEC_OPERATOR_BINARY(/);
 
    VEC_OPERATOR_INCDEC(++);
    VEC_OPERATOR_INCDEC(--);

    constexpr T Sum() const 
    {
      T out{ 0 };
      for (size_t i = 0; i < Dim; ++i)
        out += m_Buffer[i];
    }

    constexpr T Prod() const
    {
      T out{ 1 };
      for (size_t i = 0; i < Dim; ++i)
        out *= m_Buffer[i];
    }

    constexpr T Length() const { return Math::Sqrt(SqrLength()); }
    constexpr T SqrLength() const 
    { 
      T out{ 0 };
      for (size_t i = 0; i < Dim; ++i)
        out += (m_Buffer[i] * m_Buffer[i]);
    }

    Vec& Normalize() const { return ((*this) / Length()); }

    const T* Data() const { return &m_Buffer[0]; }
    constexpr size_t Size() { return Dim; }
  private:

    T m_Buffer[Dim]{};
  };

  template<typename T>
  using Vec2 = Vec<T, 2>;

  template<typename T>
  using Vec3 = Vec<T, 3>;

  template<typename T>
  using Vec4 = Vec<T, 4>;
}