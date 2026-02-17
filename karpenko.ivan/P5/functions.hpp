#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "shape.hpp"

namespace karpenko
{
  void scaleShapes(Shape** shapes, size_t count, const point_t& point, double coefficient);
  rectangle_t getOverallFrameRect(const Shape* const* shapes, size_t count);
  void printAllInfo(const Shape* const* shapes, size_t count, const char* title);
  void printFrameRectInfo(const rectangle_t& frame, const char* prefix = "");
}

#endif
