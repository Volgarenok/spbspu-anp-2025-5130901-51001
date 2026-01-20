#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include "shape.hpp"
#include <algorithm>
#include <cmath>

namespace vishnyakov
{
  struct Triangle: Shape {
    Triangle(point_t a, point_t b, point_t c);
    rectangle_t getFrameRect() const noexcept override;
    double getArea() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t newPos) noexcept override;
    void scale(double coefficient) noexcept override;
  private:
    point_t spot1_, spot2_, spot3_, center_;
  };
}

#endif