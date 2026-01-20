#include "shape.hpp"

void vishnyakov::Shape::checkedScale(double coefficient)
{
  if (coefficient <= 0.0)
  {
    throw std::invalid_argument("Error: invalid coefficient\n");
  }
  scale(coefficient);
}
