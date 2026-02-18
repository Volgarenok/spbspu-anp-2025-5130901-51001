#ifndef UTILS_HPP
#define UTILS_HPP

#include "shape.hpp"
#include <iostream>

namespace kitserov
{
  rectangle_t frame(const point_t* pts, size_t s) noexcept;
  void frameOutput(std::ostream& os, const rectangle_t& fr) noexcept;
  void shapeOutput(std::ostream& os, const Shape* sh, const char* name) noexcept;
  void printAllInfo(std::ostream& os, const Shape* const* shapes, const char* const* names, size_t count) noexcept;
  void scalePoint(Shape* sh, const point_t& p, float k);
  rectangle_t getOverallFrame(const Shape* const* shapes, size_t count) noexcept;
  point_t calculateCentroid(const point_t* vertices, size_t count) noexcept;
}

#endif
