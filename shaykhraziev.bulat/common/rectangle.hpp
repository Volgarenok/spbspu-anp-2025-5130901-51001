#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "shape.hpp"

namespace shaykhraziev
{
  struct Rectangle : Shape
  {
    point_t center;
    double width, height;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

    explicit Rectangle(point_t a, point_t b);
    explicit Rectangle(rectangle_t r);
  };
}

#endif