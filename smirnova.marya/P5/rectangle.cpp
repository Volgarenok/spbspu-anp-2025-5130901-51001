#include "rectangle.hpp"
#include <stdexcept>

#include "utils.hpp"

smirnova::Rectangle::Rectangle(double width, double height, const point_t& center):
  width_(width),
  height_(height),
  center_(center)
{
  if (width <= 0.0 || height <= 0.0)
  {
    throw std::invalid_argument("Width and height must be positive");
  }
}

double smirnova::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}
smirnova::rectangle_t smirnova::Rectangle::getFrameRect() const noexcept
{
  return makeFrameRect(width_, height_, center_);
}
void smirnova::Rectangle::move(const point_t& p) noexcept
{
  center_ = p;
}
void smirnova::Rectangle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void smirnova::Rectangle::scale(double k) noexcept
{
  width_ *= k;
  height_ *= k;
}

