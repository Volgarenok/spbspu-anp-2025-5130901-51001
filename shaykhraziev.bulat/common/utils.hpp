#ifndef UTILS_HPP
#define UTILS_HPP
#include "shape.hpp"
#include <ostream>

namespace shaykhraziev
{
  void scaleRelative(Shape& shp, point_t pt, double coef);
  rectangle_t getAllShapesFrameRect(Shape** shps, size_t size);
  void removeArray(Shape** shps, size_t size);
  void outputParams(std::ostream& out, Shape** shps, size_t size);
  double distToLine(point_t d1, point_t d2, point_t m);
  double euclidDist(point_t d1, point_t d2);
}
#endif