#include "rectangle.hpp"
#include <stdexcept>

namespace karpenko
{
  Rectangle::Rectangle(double width, double height, const point_t& center) noexcept:
    width_(width),
    height_(height),
    center_(center)
  {}

  double Rectangle::getArea() const noexcept
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    rectangle_t frame;
    frame.width = width_;
    frame.height = height_;
    frame.pos = center_;
    return frame;
  }

  void Rectangle::move(const point_t& point) noexcept
  {
    center_ = point;
  }

  void Rectangle::move(double dx, double dy) noexcept
  {
    center_.x += dx;
    center_.y += dy;
  }

  void Rectangle::doScale(double coefficient)
  {
    width_ *= coefficient;
    height_ *= coefficient;
  }
}
