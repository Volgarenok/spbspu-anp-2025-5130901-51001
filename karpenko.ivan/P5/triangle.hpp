#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"

namespace karpenko
{
  class Triangle final: public Shape
  {
  public:
    Triangle(const point_t& a, const point_t& b, const point_t& c) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void unsafeScale(double coefficient) override;

  private:
    point_t vertexA_;
    point_t vertexB_;
    point_t vertexC_;
    void moveVertices(double dx, double dy) noexcept;
    point_t getCenter() const noexcept;
    point_t getScaledVertex(const point_t& vertex, const point_t& center, double coefficient) const noexcept;
  };
}

#endif
