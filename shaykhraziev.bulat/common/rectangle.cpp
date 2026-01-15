#include "rectangle.hpp"

shaykhraziev::Rectangle::Rectangle(point_t a, point_t b) :
  center()
{
  center = {a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2};
  width = b.x - a.x;
  height = b.y - a.y;
}

shaykhraziev::Rectangle::Rectangle(rectangle_t r) :
  center(r.pos),
  width(r.width),
  height(r.height)
{}

double shaykhraziev::Rectangle::getArea() const
{
  return width * height;
}

shaykhraziev::rectangle_t shaykhraziev::Rectangle::getFrameRect() const
{
  return {width, height, center};
}

void shaykhraziev::Rectangle::move(point_t to)
{
  center = to;
}

void shaykhraziev::Rectangle::move(double dx, double dy)
{
  center.x += dx;
  center.y += dy;
}

void shaykhraziev::Rectangle::scale(double coef)
{
  width *= coef;
  height *= coef;
}