#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <cstddef>

namespace shaykhraziev
{
  struct point_t
  {
    double x, y;
  };

  bool operator==(point_t lhs, point_t rhs);
  bool operator!=(point_t lhs, point_t rhs);

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  bool operator==(const rectangle_t& lhs, const rectangle_t& rhs);
  bool operator!=(const rectangle_t& lhs, const rectangle_t& rhs);
}
#endif