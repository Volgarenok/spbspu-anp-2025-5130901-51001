#include "geometry.hpp"

bool shaykhraziev::operator==(const point_t lhs, const point_t rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool shaykhraziev::operator!=(const point_t lhs, const point_t rhs)
{
  return !(rhs == lhs);
}

bool shaykhraziev::operator==(const rectangle_t& lhs, const rectangle_t& rhs)
{
  return lhs.width == rhs.width && lhs.height == rhs.height && lhs.pos == rhs.pos;
}

bool shaykhraziev::operator!=(const rectangle_t& lhs, const rectangle_t& rhs)
{
  return !(rhs == lhs);
}