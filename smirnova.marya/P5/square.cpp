#include "square.hpp"
#include <iostream>

#include "shape.hpp"
#include "utils.hpp"

smirnova::Square::Square(double side, const point_t& center):
  side_(side),
  center_(center)
{
  if (side <= 0.0)
  {
    throw std::invalid_argument("Side must be positive");
  }
}

double smirnova::Square::getArea() const noexcept
{
  return side_ * side_;
}
smirnova::rectangle_t smirnova::Square::getFrameRect() const noexcept
{
  return makeFrameRect(side_, side_, center_);
}
void smirnova::Square::move(const point_t& p) noexcept
{
  center_ = p;
}
void smirnova::Square::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void smirnova::Square::scale(double k) noexcept
{
  side_ *= k;
}

