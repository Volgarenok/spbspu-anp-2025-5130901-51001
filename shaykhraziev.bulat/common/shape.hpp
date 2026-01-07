#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "geometry.hpp"

namespace shaykhraziev
{
  struct Shape
  {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual ~Shape() = default;
    virtual void move(point_t to) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };
}

#endif