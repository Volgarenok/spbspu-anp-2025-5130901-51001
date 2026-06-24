#ifndef POLYGON_HPP
#define POLYGON_HPP
#include "shape.hpp"

namespace shaykhraziev
{
  struct Polygon : Shape
  {
    point_t* points;
    size_t size;
    point_t center;
    double area;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

    explicit Polygon(point_t* a, size_t size);
  };
}

#endif