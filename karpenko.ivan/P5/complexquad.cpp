#include "complexquad.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace karpenko
{
  ComplexQuad::ComplexQuad(const point_t& a, const point_t& b, const point_t& c, const point_t& d) noexcept:
    vertexA_(a),
    vertexB_(b),
    vertexC_(c),
    vertexD_(d)
  {}

  double ComplexQuad::getArea() const noexcept
  {
    double area1 = triangleArea(vertexA_, vertexB_, vertexC_);
    double area2 = triangleArea(vertexA_, vertexC_, vertexD_);
    return area1 + area2;
  }

  rectangle_t ComplexQuad::getFrameRect() const noexcept
  {
    double minX = std::min({vertexA_.x, vertexB_.x, vertexC_.x, vertexD_.x});
    double maxX = std::max({vertexA_.x, vertexB_.x, vertexC_.x, vertexD_.x});
    double minY = std::min({vertexA_.y, vertexB_.y, vertexC_.y, vertexD_.y});
    double maxY = std::max({vertexA_.y, vertexB_.y, vertexC_.y, vertexD_.y});

    double width = maxX - minX;
    double height = maxY - minY;
    point_t center = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};

    rectangle_t frame;
    frame.width = width;
    frame.height = height;
    frame.pos = center;
    return frame;
  }

  void ComplexQuad::move(const point_t& point) noexcept
  {
    point_t center = getCenter();
    double dx = point.x - center.x;
    double dy = point.y - center.y;
    moveVertices(dx, dy);
  }

  void ComplexQuad::move(double dx, double dy) noexcept
  {
    moveVertices(dx, dy);
  }

  void ComplexQuad::moveVertices(double dx, double dy) noexcept
  {
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
    vertexD_.x += dx;
    vertexD_.y += dy;
  }

  point_t ComplexQuad::getCenter() const noexcept
  {
    const double epsilon = 1e-9;
    double denominator = (vertexA_.x - vertexC_.x) * (vertexB_.y - vertexD_.y)
                       - (vertexA_.y - vertexC_.y) * (vertexB_.x - vertexD_.x);

    if (std::fabs(denominator) < epsilon)
    {
      return {
        (vertexA_.x + vertexB_.x + vertexC_.x + vertexD_.x) / 4.0,
        (vertexA_.y + vertexB_.y + vertexC_.y + vertexD_.y) / 4.0
      };
    }
    else
    {
      double t = ((vertexA_.x - vertexB_.x) * (vertexB_.y - vertexD_.y)
      - (vertexA_.y - vertexB_.y) * (vertexB_.x - vertexD_.x)) / denominator;

      return {
        vertexA_.x + t * (vertexC_.x - vertexA_.x),
        vertexA_.y + t * (vertexC_.y - vertexA_.y)
      };
    }
  }

  double ComplexQuad::triangleArea(const point_t& a, const point_t& b, const point_t& c) const noexcept
  {
    double part1 = (b.x - a.x) * (c.y - a.y);
    double part2 = (c.x - a.x) * (b.y - a.y);
    return 0.5 * std::fabs(part1 - part2);
  }

  void ComplexQuad::doScale(double coefficient)
  {
    point_t center = getCenter();
    auto scaleVertex = [&center, coefficient](const point_t& vertex) {
      return point_t{
        center.x + (vertex.x - center.x) * coefficient,
        center.y + (vertex.y - center.y) * coefficient
      };
    };
    vertexA_ = scaleVertex(vertexA_);
    vertexB_ = scaleVertex(vertexB_);
    vertexC_ = scaleVertex(vertexC_);
    vertexD_ = scaleVertex(vertexD_);
  }
}
