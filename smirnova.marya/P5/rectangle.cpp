#include "rectangle.hpp"
#include <stdexcept>

#include "utils.hpp"

namespace smirnova
{
  Rectangle::Rectangle(double width, double height, const point_t &center)
      : width_(width), height_(height), center_(center)
  {
    if (width <= 0.0 || height <= 0.0)
    {
      throw std::invalid_argument("Width and height must be positive");
    }
  }

  double Rectangle::getArea() const noexcept { return width_ * height_; }
  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    return makeFrameRect(width_, height_, center_);
  }
  void Rectangle::move(const point_t &p) noexcept { moveToPoint(center_, p); }
  void Rectangle::move(double dx, double dy) noexcept { moveCenter(center_, dx, dy); }
  void Rectangle::scale(double k) noexcept
  {
    width_ *= k;
    height_ *= k;
  }
} // namespace smirnova
