#include "concave.hpp"

vishnyakov::Concave::Concave(point_t a, point_t b, point_t c, point_t d) noexcept:
  spot1_{a},
  spot2_{b},
  spot3_{c},
  spot4_{d},
  center_{point_t{(a.x + b.x + c.x + d.x) / 4.0, (a.y + b.y + c.y + d.y) / 4.0}}
{}

vishnyakov::rectangle_t vishnyakov::Concave::getFrameRect() const noexcept
{
  double minx = std::min({spot1_.x, spot2_.x, spot3_.x, spot4_.x});
  double maxx = std::max({spot1_.x, spot2_.x, spot3_.x, spot4_.x});
  double miny = std::min({spot1_.y, spot2_.y, spot3_.y, spot4_.y});
  double maxy = std::max({spot1_.y, spot2_.y, spot3_.y, spot4_.y});

  double width_ = maxx - minx;
  double height = maxy - miny;

  point_t center = {(maxx - minx) / 2.0, (maxy - miny) / 2.0};

  return rectangle_t{width_, height, center};
}

double vishnyakov::Concave::getArea() const noexcept
{
  double tmp_value_1 = (spot1_.x - spot2_.x) * (spot1_.y + spot2_.y);
  double tmp_value_2 = (spot2_.x - spot3_.x) * (spot2_.y + spot3_.y);
  double tmp_value_3 = (spot3_.x - spot4_.x) * (spot3_.y + spot4_.y);
  double tmp_value_4 = (spot4_.x - spot1_.x) * (spot4_.y + spot1_.y);

  double result = std::abs(tmp_value_1 + tmp_value_2 + tmp_value_3 + tmp_value_4);

  return result;
}

void vishnyakov::Concave::move(double dx, double dy) noexcept
{
  movePoint(spot1_, dx, dy);
  movePoint(spot2_, dx, dy);
  movePoint(spot3_, dx, dy);
  movePoint(spot4_, dx, dy);

  movePoint(center_, dx, dy);
}

void vishnyakov::Concave::move(point_t newPos) noexcept
{
  double dx = newPos.x - center_.x;
  double dy = newPos.y - center_.y;

  move(dx, dy);
}

void vishnyakov::Concave::scale(double coefficient) noexcept
{
  spot1_.x = center_.x + (spot1_.x - center_.x) * coefficient;
  spot2_.x = center_.x + (spot2_.x - center_.x) * coefficient;
  spot3_.x = center_.x + (spot3_.x - center_.x) * coefficient;
  spot4_.x = center_.x + (spot4_.x - center_.x) * coefficient;

  spot1_.y = center_.y + (spot1_.y - center_.y) * coefficient;
  spot2_.y = center_.y + (spot2_.y - center_.y) * coefficient;
  spot3_.y = center_.y + (spot3_.y - center_.y) * coefficient;
  spot4_.y = center_.y + (spot4_.y - center_.y) * coefficient;
}
