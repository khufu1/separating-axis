#include "Collider.hpp"

namespace sat {

bool Collider::collidingWith(Collider &c) noexcept
{
  return SAT(vertices(), c.vertices());
}

const vec2f Collider::computeEdgeNormal(const vec2f &pointA,
                                        const vec2f &pointB) noexcept
{
  // terminal - initial -> <(pointB.x -pointA.x) , (pointB.y - pointA.y)>
  vec2f edgeVector = {pointB.x - pointA.x, pointB.y - pointA.y};
  float magnitude = hypotf(edgeVector.x, edgeVector.y); // AKA length
  if (magnitude != 0) [[likely]]
    // we devide by magnitude to convert it to a unit vec
    // also called normalization
    return {-edgeVector.y / magnitude, edgeVector.x / magnitude};
  else
    return {-edgeVector.y, edgeVector.x};
}

const std::vector<vec2f>
Collider::computeAllNormals(const std::vector<vec2f> &r) noexcept
{
  std::vector<vec2f> normals;

  switch (Shape) {
  case Shapes::Rectangle:
    normals.push_back(computeEdgeNormal(r.at(1), r.at(2)));
    normals.push_back(computeEdgeNormal(r.at(0), r.at(1)));
    break;

  default:
    for (size_t i = 0; i < r.size(); i++) {
      normals.push_back(
          computeEdgeNormal(r.at(i), r.at((i + 1 != r.size()) ? i + 1 : 0)));
    }
    break;
  }
  return normals;
}

const vec2f Collider::project(const std::vector<vec2f> &r,
                              const vec2f &axis) noexcept
{
  float min = r.at(0).dot(axis);
  float max = min;
  float projection;

  for (auto &cp : r) {
    projection = cp.dot(axis);

    if (projection > max) {
      max = projection;
    }
    if (projection < min) {
      min = projection;
    }
  }
  return {min, max};
}

// true if overlapping false otherwise
FORCE_INLINE_ bool Collider::overlap(const vec2f &projection1,
                                     const vec2f &projection2) noexcept
{
  return std::min(projection1.x, projection1.y) <
             std::max(projection2.x, projection2.y) &&
         std::min(projection2.x, projection2.y) <
             std::max(projection1.x, projection1.y);
}

bool Collider::SAT(const std::vector<vec2f> &r1,
                   const std::vector<vec2f> &r2) noexcept
{
  std::vector normals = computeAllNormals(r1);
  bool flippedNormals{false};
  bool overlapping;

loop:
  for (auto n : normals) {
    vec2f p1 = project(r1, n);
    vec2f p2 = project(r2, n);

    overlapping = overlap(p1, p2);
    if (!overlapping)
      return false;
  }

  if (!flippedNormals) {
    normals = computeAllNormals(r2);
    flippedNormals = true;
    goto loop;
  }
  else {
    return true;
  }
}

} // namespace sat
