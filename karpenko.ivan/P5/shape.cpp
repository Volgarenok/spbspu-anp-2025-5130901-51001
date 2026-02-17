#include "shape.hpp"

void karpenko::Shape::scale(double coefficient)
{
  if (coefficient <= 0.0)
  {
    throw std::invalid_argument("Scaling coefficient must be positive");
  }
  unsafeScale(coefficient);
}
