#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"

namespace smirnova
{
  class Triangle final: public Shape
  {
  public:
    Triangle(point_t a, point_t b, point_t c);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) noexcept override;

  private:
    point_t a_, b_, c_;
  };
}

#endif

