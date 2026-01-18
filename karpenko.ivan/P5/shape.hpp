#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <stdexcept>

namespace karpenko
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  struct Shape
  {
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& point) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void scale(double coefficient) = 0;
  };
}

#endif
