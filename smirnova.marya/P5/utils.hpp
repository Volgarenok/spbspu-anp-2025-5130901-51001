#ifndef UTILS_HPP
#define UTILS_HPP

#include "shape.hpp"

namespace smirnova
{
  rectangle_t makeFrameRect(double w, double h, const point_t& center) noexcept;
  point_t getCentroid(const point_t& a, const point_t& b, const point_t& c) noexcept;
}

#endif

