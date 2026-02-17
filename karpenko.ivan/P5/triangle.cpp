#include "triangle.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

karpenko::Triangle::Triangle(const point_t& a, const point_t& b, const point_t& c) noexcept:
  vertexA_(a),
  vertexB_(b),
  vertexC_(c)
{}

double karpenko::Triangle::getArea() const noexcept
{
  double bx_ax = vertexB_.x - vertexA_.x;
  double cy_ay = vertexC_.y - vertexA_.y;
  double cx_ax = vertexC_.x - vertexA_.x;
  double by_ay = vertexB_.y - vertexA_.y;

  double part1 = bx_ax * cy_ay;
  double part2 = cx_ax * by_ay;
  return 0.5 * std::fabs(part1 - part2);
}

karpenko::rectangle_t karpenko::Triangle::getFrameRect() const noexcept
{
  double minX = std::min({vertexA_.x, vertexB_.x, vertexC_.x});
  double maxX = std::max({vertexA_.x, vertexB_.x, vertexC_.x});
  double minY = std::min({vertexA_.y, vertexB_.y, vertexC_.y});
  double maxY = std::max({vertexA_.y, vertexB_.y, vertexC_.y});

  double width = maxX - minX;
  double height = maxY - minY;
  double centerX = (minX + maxX) / 2.0;
  double centerY = (minY + maxY) / 2.0;

  return {width, height, {centerX, centerY}};
}

void karpenko::Triangle::move(const point_t& point) noexcept
{
  point_t center = getCenter();
  double dx = point.x - center.x;
  double dy = point.y - center.y;
  moveVertices(dx, dy);
}

void karpenko::Triangle::move(double dx, double dy) noexcept
{
  moveVertices(dx, dy);
}

void karpenko::Triangle::moveVertices(double dx, double dy) noexcept
{
  vertexA_.x += dx;
  vertexA_.y += dy;
  vertexB_.x += dx;
  vertexB_.y += dy;
  vertexC_.x += dx;
  vertexC_.y += dy;
}

karpenko::point_t karpenko::Triangle::getCenter() const noexcept
{
  double sumX = vertexA_.x + vertexB_.x + vertexC_.x;
  double sumY = vertexA_.y + vertexB_.y + vertexC_.y;
  return {sumX / 3.0, sumY / 3.0};
}

karpenko::point_t karpenko::Triangle::getScaledVertex(const point_t& vertex,
  const point_t& center, double coefficient) const noexcept
{
  double dx = vertex.x - center.x;
  double dy = vertex.y - center.y;
  double scaledX = center.x + dx * coefficient;
  double scaledY = center.y + dy * coefficient;
  return {scaledX, scaledY};
}

void karpenko::Triangle::unsafeScale(double coefficient)
{
  point_t center = getCenter();
  vertexA_ = getScaledVertex(vertexA_, center, coefficient);
  vertexB_ = getScaledVertex(vertexB_, center, coefficient);
  vertexC_ = getScaledVertex(vertexC_, center, coefficient);
}
