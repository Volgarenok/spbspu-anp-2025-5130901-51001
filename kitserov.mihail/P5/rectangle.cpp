#include "rectangle.hpp"

kitserov::Rectangle::Rectangle(point_t p, float w, float h) noexcept:
  rect_{w, h, p}
{}

float kitserov::Rectangle::getArea() const noexcept
{
  return rect_.width * rect_.height;
}

kitserov::rectangle_t kitserov::Rectangle::getFrameRect() const noexcept
{
  return rect_;
}

void kitserov::Rectangle::move(float dx, float dy) noexcept
{
  rect_.pos.x += dx;
  rect_.pos.y += dy;
}

void kitserov::Rectangle::move(const point_t& p) noexcept
{
  rect_.pos = p;
}

void kitserov::Rectangle::doScale(float k)
{
  rect_.width *= k;
  rect_.height *= k;
}
