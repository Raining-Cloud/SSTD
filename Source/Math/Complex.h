#pragma once

#include "Math.h"

namespace SSTD
{
  template<typename T>
  class Complex
  {
  public:
    Complex(const T real, const T imaginary) :re(real), im(imaginary) {}
    Complex(const Complex& other) {}
    Complex(Complex&& other) {}
    
    ~Complex() {}

    T Real() const { return re; }
    T Imag() const { return im; }

    T Norm() const { return Math::Sqrt(re * re + im * im); }
  private:
    T re{};
    T im{};
  };
}