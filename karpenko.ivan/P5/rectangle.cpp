#include "rectangle.hpp"
#include <stdexcept>

karpenko::Rectangle::Rectangle(double width, double height, const point_t& center) noexcept:
  width_(width),
  height_(height),
  center_(center)
{}

double karpenko::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

karpenko::rectangle_t karpenko::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, center_};
}

void karpenko::Rectangle::move(const point_t& point) noexcept
{
  center_ = point;
}

void karpenko::Rectangle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}

void karpenko::Rectangle::unsafeScale(double coefficient)
{
  width_ *= coefficient;
  height_ *= coefficient;
}
