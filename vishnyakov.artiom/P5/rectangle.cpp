#include "rectangle.hpp"

vishnyakov::Rectangle::Rectangle(double w, double h, point_t c):
  width_{w},
  height_{h},
  center_{c}
{}

vishnyakov::rectangle_t vishnyakov::Rectangle::getFrameRect() const noexcept
{
  return rectangle_t{width_, height_, center_};
}

double vishnyakov::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

void vishnyakov::Rectangle::move(double dx, double dy) noexcept
{
  movePoint(center_, dx, dy);
}

void vishnyakov::Rectangle::move(point_t newPos) noexcept
{
  center_ = newPos;
}

void vishnyakov::Rectangle::scale(double coefficient) noexcept
{
  width_ *= coefficient;
  height_ *= coefficient;
}