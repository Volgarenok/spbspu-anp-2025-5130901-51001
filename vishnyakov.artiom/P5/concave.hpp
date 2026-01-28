#ifndef CONCAVE_HPP
#define CONCAVE_HPP
#include <algorithm>
#include "shape.hpp"
#include "functions.hpp"

namespace vishnyakov
{
  struct Concave final: Shape {
    Concave(point_t a, point_t b, point_t c, point_t d) noexcept;
    rectangle_t getFrameRect() const noexcept override;
    double getArea() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t newPos) noexcept override;
    void scale(double coefficient) noexcept override;

  private:
    point_t spot1_, spot2_, spot3_, spot4_, center_;
  };
}

#endif
