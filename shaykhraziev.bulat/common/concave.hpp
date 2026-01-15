#ifndef CONCAVE_HPP
#define CONCAVE_HPP
#include "shape.hpp"

namespace shaykhraziev
{
  struct Concave : Shape
  {
    point_t points[4];
    point_t center;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

    explicit Concave(const point_t* a, size_t size);
  };
}

#endif