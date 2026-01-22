#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "shape.hpp"
#include "functions.hpp"

namespace vishnyakov
{
  struct Rectangle final: Shape {
    Rectangle(double w, double h, point_t c);
    rectangle_t getFrameRect() const noexcept override;
    double getArea() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(point_t newPos) noexcept override;
    void scale(double coefficient) noexcept override;
  private:
    double width_, height_;
    point_t center_;
  };   
}

#endif

