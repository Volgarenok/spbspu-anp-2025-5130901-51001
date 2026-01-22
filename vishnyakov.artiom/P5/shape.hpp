#ifndef SHAPE_HPP
#define SHAPE_HPP
#include <iostream>
#include "rectangle_t.hpp"

namespace vishnyakov
{
  struct Shape {
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void move(const point_t newPos) noexcept = 0;
    virtual void scale(double coefficient) noexcept = 0;
    void checkedScale(double coefficient);
  };
}

#endif

