#include "xquare.hpp"


kitserov::Xquare::Xquare(point_t p, float s) noexcept:
  centre_(p),
  side_(s)
{}

float kitserov::Xquare::getArea() const noexcept
{
  return 0.5 * side_ * side_;
}

kitserov::rectangle_t kitserov::Xquare::getFrameRect() const noexcept
{
  return {side_, side_, centre_};
}

void kitserov::Xquare::move(float dx, float dy) noexcept
{
  centre_.x += dx;
  centre_.y += dy;
}

void kitserov::Xquare::move(const point_t& p) noexcept
{
  centre_ = p;
}

void kitserov::Xquare::doScale(float k)
{
  side_ *= k;
}
