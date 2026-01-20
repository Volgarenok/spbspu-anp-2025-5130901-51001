#include "triangle.hpp"

vishnyakov::Triangle::Triangle(point_t a, point_t b, point_t c):
  spot1_{a},
  spot2_{b},
  spot3_{c},
  center_{point_t {(a.x + b.x + c.x) / 3.0, (a.y + b.y + c.y) / 3.0}}
{}

vishnyakov::rectangle_t vishnyakov::Triangle::getFrameRect() const noexcept
{
  double minx = std::min({spot1_.x, spot2_.x, spot3_.x});
  double maxx = std::max({spot1_.x, spot2_.x, spot3_.x});
  double miny = std::min({spot1_.y, spot2_.y, spot3_.y});
  double maxy = std::max({spot1_.y, spot2_.y, spot3_.y});

  double width_ = maxx - minx;
  double height = maxy - miny;

  point_t center = {(minx + maxx) / 2.0, (miny + maxy) / 2.0};

  return rectangle_t{width_, height, center};
}

double vishnyakov::Triangle::getArea() const noexcept
{
  double side1 = 0, side2 = 0 , side3 = 0, p = 0, result = 0;

  side1 = std::sqrt((spot1_.x - spot2_.x) * (spot1_.x - spot2_.x) + (spot1_.y - spot2_.y) * (spot1_.y - spot2_.y));
  side2 = std::sqrt((spot2_.x - spot3_.x) * (spot2_.x - spot3_.x) + (spot2_.y - spot3_.y) * (spot2_.y - spot3_.y));
  side3 = std::sqrt((spot1_.x - spot3_.x) * (spot1_.x - spot3_.x) + (spot1_.y - spot3_.y) * (spot1_.y - spot3_.y));

  p = 0.5 * (side1 + side2 + side3);

  result = std::sqrt(p * (p - side1) * (p - side2) * (p - side3));

  return result;
}

void vishnyakov::Triangle::move(double dx, double dy) noexcept
{
  movePoint(spot1_, dx, dy);
  movePoint(spot2_, dx, dy);
  movePoint(spot3_, dx, dy);

  movePoint(center_, dx, dy);
}

void vishnyakov::Triangle::move(const point_t newPos) noexcept
{
  double dx = newPos.x - center_.x;
  double dy = newPos.y - center_.y;

  move(dx, dy);
}

void vishnyakov::Triangle::scale(double coefficient) noexcept
{
  spot1_.x = center_.x + (spot1_.x - center_.x) * coefficient;
  spot2_.x = center_.x + (spot2_.x - center_.x) * coefficient;
  spot3_.x = center_.x + (spot3_.x - center_.x) * coefficient;

  spot1_.y = center_.y + (spot1_.y - center_.y) * coefficient;
  spot2_.y = center_.y + (spot2_.y - center_.y) * coefficient;
  spot3_.y = center_.y + (spot3_.y - center_.y) * coefficient;
}