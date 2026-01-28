#include "triangle.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>

#include "utils.hpp"

smirnova::Triangle::Triangle(point_t a, point_t b, point_t c):
  a_(a),
  b_(b),
  c_(c)
{
  if (getArea() <= 0.0)
  {
    throw std::invalid_argument("Points do not form a valid triangle");
  }
}

double smirnova::Triangle::getArea() const noexcept
{
  double term1 = a_.x * (b_.y - c_.y);
  double term2 = b_.x * (c_.y - a_.y);
  double term3 = c_.x * (a_.y - b_.y);
  double result = term1 + term2 + term3;
  return 0.5 * std::abs(result);
}

smirnova::rectangle_t smirnova::Triangle::getFrameRect() const noexcept
{
  const double minX = std::min({a_.x, b_.x, c_.x});
  const double maxX = std::max({a_.x, b_.x, c_.x});
  const double minY = std::min({a_.y, b_.y, c_.y});
  const double maxY = std::max({a_.y, b_.y, c_.y});
  return makeFrameRect(maxX - minX, maxY - minY, {(minX + maxX) / 2.0, (minY + maxY) / 2.0});
}

void smirnova::Triangle::move(const point_t& p) noexcept
{
  point_t centroid = getCentroid(a_, b_, c_);
  double dx = p.x - centroid.x;
  double dy = p.y - centroid.y;
  Triangle::move(dx, dy);
}

void smirnova::Triangle::move(double dx, double dy) noexcept
{
  a_.x += dx;
  a_.y += dy;
  b_.x += dx;
  b_.y += dy;
  c_.x += dx;
  c_.y += dy;
}

void smirnova::Triangle::scale(double k) noexcept
{
  point_t center = getCentroid(a_, b_, c_);
  a_.x = center.x + (a_.x - center.x) * k;
  a_.y = center.y + (a_.y - center.y) * k;
  b_.x = center.x + (b_.x - center.x) * k;
  b_.y = center.y + (b_.y - center.y) * k;
  c_.x = center.x + (c_.x - center.x) * k;
  c_.y = center.y + (c_.y - center.y) * k;
}

