#include "utils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace smirnova
{
  void moveCenter(point_t &center, const double dx, const double dy)
  {
    center.x += dx;
    center.y += dy;
  }

  void moveToPoint(point_t &center, const point_t &p)
  {
    center = p;
  }

  rectangle_t makeFrameRect(double w, double h,
    const point_t &center) noexcept
  {
    return {w, h, center};
  }

  point_t getCentroid(const point_t &a, const point_t &b,
    const point_t &c) noexcept
  {
    return {(a.x + b.x + c.x) / 3.0, (a.y + b.y + c.y) / 3.0};
  }
}
