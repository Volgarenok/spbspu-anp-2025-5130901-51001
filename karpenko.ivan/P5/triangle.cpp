#include "triangle.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace karpenko
{
  Triangle::Triangle(const point_t& a, const point_t& b, const point_t& c) noexcept:
    vertexA_(a),
    vertexB_(b),
    vertexC_(c)
  {}

  double Triangle::getArea() const noexcept
  {
    double part1 = (vertexB_.x - vertexA_.x) * (vertexC_.y - vertexA_.y);
    double part2 = (vertexC_.x - vertexA_.x) * (vertexB_.y - vertexA_.y);
    return 0.5 * std::fabs(part1 - part2);
  }

  rectangle_t Triangle::getFrameRect() const noexcept
  {
    double minX = std::min({vertexA_.x, vertexB_.x, vertexC_.x});
    double maxX = std::max({vertexA_.x, vertexB_.x, vertexC_.x});
    double minY = std::min({vertexA_.y, vertexB_.y, vertexC_.y});
    double maxY = std::max({vertexA_.y, vertexB_.y, vertexC_.y});

    double width = maxX - minX;
    double height = maxY - minY;
    point_t center = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};

    rectangle_t frame;
    frame.width = width;
    frame.height = height;
    frame.pos = center;
    return frame;
  }

  void Triangle::move(const point_t& point) noexcept
  {
    point_t center = getCenter();
    double dx = point.x - center.x;
    double dy = point.y - center.y;
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
  }

  void Triangle::move(double dx, double dy) noexcept
  {
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
  }

  point_t Triangle::getCenter() const noexcept
  {
    return {
      (vertexA_.x + vertexB_.x + vertexC_.x) / 3.0,
      (vertexA_.y + vertexB_.y + vertexC_.y) / 3.0
    };
  }

  point_t Triangle::getScaledVertex(const point_t& vertex, const point_t& center, double coefficient) const noexcept
  {
    return {
      center.x + (vertex.x - center.x) * coefficient,
      center.y + (vertex.y - center.y) * coefficient
    };
  }

  void Triangle::doScale(double coefficient)
  {
    point_t center = getCenter();
    vertexA_ = getScaledVertex(vertexA_, center, coefficient);
    vertexB_ = getScaledVertex(vertexB_, center, coefficient);
    vertexC_ = getScaledVertex(vertexC_, center, coefficient);
  }
}
