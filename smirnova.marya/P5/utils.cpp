#include "utils.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

smirnova::rectangle_t smirnova::makeFrameRect(double w, double h, const point_t& center) noexcept
{
  return {w, h, center};
}

smirnova::point_t smirnova::getCentroid(const point_t& a, const point_t& b, const point_t& c) noexcept
{
  return {(a.x + b.x + c.x) / 3.0, (a.y + b.y + c.y) / 3.0};
}

