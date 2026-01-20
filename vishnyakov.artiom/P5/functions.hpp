#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <iostream>
#include "shape.hpp"

namespace vishnyakov
{
  void movePoint(point_t &pos, double dx, double dy);
  rectangle_t getFullFrameRect(const Shape* const *shapes, size_t size);
  void scaleAllShapes(Shape **shapes, size_t size, point_t point, double coefficient);
  void outputInformation(const Shape* const *shapes, size_t size);
}

#endif