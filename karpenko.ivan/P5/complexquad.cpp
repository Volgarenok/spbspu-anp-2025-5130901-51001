#include "complexquad.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

karpenko::ComplexQuad::ComplexQuad(const point_t& a, const point_t& b, const point_t& c, const point_t& d) noexcept:
  vertexA_(a),
  vertexB_(b),
  vertexC_(c),
  vertexD_(d)
{}

double karpenko::ComplexQuad::getArea() const noexcept
{
  double area1 = triangleArea(vertexA_, vertexB_, vertexC_);
  double area2 = triangleArea(vertexA_, vertexC_, vertexD_);
  return area1 + area2;
}

karpenko::rectangle_t karpenko::ComplexQuad::getFrameRect() const noexcept
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

void karpenko::ComplexQuad::move(const point_t& point) noexcept
{
  point_t center = getCenter();
  double dx = point.x - center.x;
  double dy = point.y - center.y;
  moveVertices(dx, dy);
}

void karpenko::ComplexQuad::move(double dx, double dy) noexcept
{
  moveVertices(dx, dy);
}

void karpenko::ComplexQuad::moveVertices(double dx, double dy) noexcept
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

karpenko::point_t karpenko::ComplexQuad::getCenter() const noexcept
{
  const double epsilon = 1e-9;
  
  double ac_x = vertexA_.x - vertexC_.x;
  double ac_y = vertexA_.y - vertexC_.y;
  double bd_x = vertexB_.x - vertexD_.x;
  double bd_y = vertexB_.y - vertexD_.y;
  
  double denominator = ac_x * bd_y - ac_y * bd_x;

  if (std::fabs(denominator) < epsilon)
  {
    double sumX = vertexA_.x + vertexB_.x + vertexC_.x + vertexD_.x;
    double sumY = vertexA_.y + vertexB_.y + vertexC_.y + vertexD_.y;
    return {sumX / 4.0, sumY / 4.0};
  }
  else
  {
    double ab_x = vertexA_.x - vertexB_.x;
    double ab_y = vertexA_.y - vertexB_.y;
    
    double numerator = ab_x * bd_y - ab_y * bd_x;
    double t = numerator / denominator;

    double cx = vertexC_.x - vertexA_.x;
    double cy = vertexC_.y - vertexA_.y;
    
    return {
      vertexA_.x + t * cx,
      vertexA_.y + t * cy
    };
  }
}

double karpenko::ComplexQuad::triangleArea(const point_t& a, const point_t& b, const point_t& c) const noexcept
{
  double part1 = (b.x - a.x) * (c.y - a.y);
  double part2 = (c.x - a.x) * (b.y - a.y);
  return 0.5 * std::fabs(part1 - part2);
}

void karpenko::ComplexQuad::doScale(double coefficient)
{
  point_t center = getCenter();
  auto scaleVertex = [&center, coefficient](const point_t& vertex) {
    double dx = vertex.x - center.x;
    double dy = vertex.y - center.y;
    return point_t{
      center.x + dx * coefficient,
      center.y + dy * coefficient
    };
  };
  vertexA_ = scaleVertex(vertexA_);
  vertexB_ = scaleVertex(vertexB_);
  vertexC_ = scaleVertex(vertexC_);
  vertexD_ = scaleVertex(vertexD_);
}
