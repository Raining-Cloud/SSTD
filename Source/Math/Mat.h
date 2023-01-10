#pragma once

#include "General/Memory.h"
#include "General/Numeric.h"
#include "Vec.h"

namespace SSTD
{
  /*
   #  ->  N
   |
   v
   M
  */


  template<typename T, size_t M, size_t N>
  struct Mat
  {
  private:
    struct AccsessProxy
    {
      AccsessProxy(T* data) : ptr(data) {}
      AccsessProxy(const AccsessProxy&) = delete;
      AccsessProxy(AccsessProxy&&) = delete;

      AccsessProxy& operator=(const AccsessProxy&) = delete;
      AccsessProxy& operator=(AccsessProxy&&) = delete;

      T& operator[](size_t index) { return ptr[index]; }
    private:
      T* ptr;
    };
  public:
    static constexpr size_t size = N * M;
    explicit Mat() {}

    template<typename ... Args>
    Mat(Args&&... args) : data{ args... } {}

    constexpr Vec<T, N> Row(size_t n) const
    {
      return { data[n] };
    }

    constexpr void SetRow(size_t n, const Vec<T, N>& vec)
    {
      TMemCpy<T>(data[n], vec.data, N);
    }

    constexpr Vec<T, M> Column(size_t m) const
    {
      Vec<T, M> out{};

      for (size_t i = 0; i < M; i++)
        new (out.data[i]) T{ data[i][m] };

      return out;
    }

    constexpr Vec<T, M> SetColumn(size_t m, const Vec<T, M> vec)
    {
      for (size_t i = 0; i < M; i++)
        new (data[i][m]) T{ vec[i] };
    }

    constexpr T& At(size_t m, size_t n)
    {
      if (m < M && n < N)
        return data[m][n];
      else
        throw;
    }

    constexpr AccsessProxy operator[](size_t m) { return AccsessProxy{ data[m] }; }

    template<size_t P>
    Mat<T, M, P> operator*(const Mat<T, N, P>& other)
    {
      Mat<T, M, P> out{};

      for (size_t i = 0; i < M; i++)
        for (size_t j = 0; j < P; j++)
          for (size_t k = 0; k < N; k++)
            out.data[i][j] += data[i][k] * other.data[k][j];

      return out;
    }

#define MAT_OPERATOR_ASSIGNMENT(op)                 \
    Mat& operator op (const Mat& other)             \
    {                                               \
      for (size_t i = 0; i < M; i++)                \
        for (size_t j = 0; j < N; j++)              \
          data[i][j] op other.data[i][j];           \
      return *this;                                 \
    }                                               \
                                                    \
    constexpr Mat& operator op (const T& scalar)    \
    {                                               \
      for (size_t i = 0; i < M; i++)                \
        for (size_t j = 0; j < N; j++)              \
           data[i][j] op scalar;                    \
      return *this;                                 \
    }                                               \

#define MAT_OPERATOR_BINARY(op)                       \
    Mat operator op (const Mat& other)                \
    {                                                 \
      Mat out{};                                      \
      for (size_t i = 0; i < M; i++)                  \
        for (size_t j = 0; j < N; j++)                \
          out[i][j] = data[i][j] op other.data[i][j]; \
      return out;                                     \
    }                                                 \
    constexpr Mat& operator op (const T& scalar)      \
    {                                                 \
      Mat out;                                        \
      for (size_t i = 0; i < M; i++)                  \
        for (size_t j = 0; j < N; j++)                \
           out.data[i][j] = data[i][j] op scalar;     \
      return out;                                     \
    }                                                 \

#define MAT_OPERATOR_UNARY(op)          \
    constexpr Mat& operator op()        \
    {                                   \
      for (size_t i = 0; i < M; i++)    \
        for (size_t j = 0; j < N; j++)  \
          data[i][j] = op data[i][j];   \
      return *this;                     \
    }                                   \

#define MAT_OPERATOR_COMPARISON(op)                         \
    constexpr Mat<bool, M, N>operator op (const Mat& other) \
    {                                                       \
      Mat<bool, M, N> out{};                                \
      for (size_t i = 0; i < M; i++)                        \
        for (size_t j = 0; j < N; j++)                      \
          out.data[i][j] = data[i][j] op other.data[i][j];  \
    }                                                       \

    MAT_OPERATOR_UNARY(!);
    MAT_OPERATOR_UNARY(~);
    MAT_OPERATOR_UNARY(-);

    MAT_OPERATOR_COMPARISON(== );
    MAT_OPERATOR_COMPARISON(!= );
    MAT_OPERATOR_COMPARISON(< );
    MAT_OPERATOR_COMPARISON(> );
    MAT_OPERATOR_COMPARISON(>= );
    MAT_OPERATOR_COMPARISON(<= );

    MAT_OPERATOR_ASSIGNMENT(+= );
    MAT_OPERATOR_ASSIGNMENT(-= );

    MAT_OPERATOR_BINARY(+);
    MAT_OPERATOR_BINARY(-);

    T data[M][N]{};
  };


  template<typename T>
  using Mat2 = Mat<T, 2, 2>;

  template<typename T>
  using Mat3 = Mat<T, 3, 3>;

  template<typename T>
  using Mat4 = Mat<T, 4, 4>;
}