#include "square.hpp"
#include <iostream>

#include "shape.hpp"
#include "utils.hpp"

namespace smirnova {
Square::Square(double side, const point_t &center)
    : side_(side), center_(center) {
  if (side <= 0.0)
    throw std::invalid_argument("Side must be positive");
}

double Square::getArea() const noexcept { return side_ * side_; }
rectangle_t Square::getFrameRect() const noexcept {
  return makeFrameRect(side_, side_, center_);
}
void Square::move(const point_t &p) noexcept { moveToPoint(center_, p); }
void Square::move(double dx, double dy) noexcept {
  moveCenter(center_, dx, dy);
}
void Square::scale(double k) noexcept { side_ *= k; }
} // namespace smirnova
