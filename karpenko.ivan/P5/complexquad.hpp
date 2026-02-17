#ifndef COMPLEXQUAD_HPP
#define COMPLEXQUAD_HPP

#include "shape.hpp"

namespace karpenko
{
  class ComplexQuad final: public Shape
  {
  public:
    ComplexQuad(const point_t& a, const point_t& b, const point_t& c, const point_t& d) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void unsafeScale(double coefficient) noexcept override;

  private:
    point_t vertexA_;
    point_t vertexB_;
    point_t vertexC_;
    point_t vertexD_;
    void moveVertices(double dx, double dy) noexcept;
    point_t getCenter() const noexcept;
    point_t getScaledVertex(const point_t& vertex, const point_t& center, double coefficient) const noexcept;
    double triangleArea(const point_t& a, const point_t& b, const point_t& c) const noexcept;
  };
}

#endif
