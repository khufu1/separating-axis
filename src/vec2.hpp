#pragma once

#include "PCH.hpp"
#include "global.h"

#define RADIANS(A) (A * std::numbers::pi) / 180

namespace math {

template <typename T>
struct vec2 {
  static_assert(std::is_arithmetic<T>(), "Only arithmetic types allowed!!");
  T x;
  T y;

  /*
    The dot product of two vectors is equal to the scalar product of their
    lengths times the cosine of the angle between them. If this sounds
    confusing take a look at its formula: ̄v · ̄k = || ̄v||·|| ̄k||·cos θ
  */
  FORCE_INLINE_ T dot(const vec2 v) const
  {
    return (x * v.x) + (y * v.y);
  }

  /*
    To rotate the vector counterclockwise in a
    right-handed coordinate system, you can use the following formulas:
    - new_x = x * cos(theta) + y * sin(theta)
    - new_y = -x * sin(theta) + y * cos(theta)

    If you want to rotate the vector clockwise, you can use the following
    formulas:
    - new_x = x * cos(theta) - y * sin(theta)
    - new_y = x * sin(theta) + y * cos(theta)
    this function rotates clockwise right handed coordinate system
  */
  inline void rotate(double angle, vec2 center) noexcept
  {
    *this = *this - center;
    float r = RADIANS(angle);
    *this = {this->x * cosf(r) - this->y * sinf(r),
             this->x * sinf(r) + this->y * cosf(r)};
    *this += center;
  }

  inline void translate(vec2 v) noexcept
  {
    *this += v;
  }

  FORCE_INLINE_ vec2 operator+(const vec2 v) const noexcept
  {
    return {x + v.x, y + v.y};
  }

  FORCE_INLINE_ void operator+=(const vec2 v) noexcept
  {
    *this = {*this + v};
  }

  FORCE_INLINE_ vec2 operator-(vec2 v) const noexcept
  {
    return {x - v.x, y - v.y};
  }

  FORCE_INLINE_ void operator-=(const vec2 v) noexcept
  {
    *this = {*this - v};
  }
};

using vec2f = vec2<float>;

template <typename T>
FORCE_INLINE_ void operator<<(std::ostream &s, vec2<T> &v)
{
  s << std::format("<{},{}>", v.x, v.y) << std::endl;
}

} // namespace math
