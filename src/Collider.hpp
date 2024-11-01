#pragma once

#include "Drawing.hpp"
#include "global.h" // IWYU pragma: export
#include "vec2.hpp"
#include <SDL3/SDL.h>

using namespace math;

namespace sat {

class Collider : public Drawing {
public:
  using Drawing::Drawing;
  [[nodiscard]] bool collidingWith(const Collider &c) noexcept;

private:
  const vec2f computeEdgeNormal(const vec2f &pointA, const vec2f &pointB) noexcept;
  const std::vector<vec2f> computeAllNormals(const std::vector<vec2f> &r) noexcept;
  const vec2f project(const std::vector<vec2f> &r, const vec2f &axis) noexcept;
  // true if overlapping false otherwise
  bool overlap(const vec2f &projection1, const vec2f &projection2) noexcept;
  bool SAT(const std::vector<vec2f> &r1, const std::vector<vec2f> &r2) noexcept;
};

} // namespace sat
