#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

namespace smirnova
{
  class Rectangle final: public Shape
  {
  public:
    Rectangle(double width, double height, const point_t& center);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) noexcept override;

  private:
    double width_, height_;
    point_t center_;
  };
}

#endif

