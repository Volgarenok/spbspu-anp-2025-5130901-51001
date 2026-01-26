#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

namespace karpenko
{
  class Rectangle final: public Shape
  {
  public:
    Rectangle(double width, double height, const point_t& center) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void unsafeScale(double coefficient) override;

  private:
    double width_;
    double height_;
    point_t center_;
  };
}

#endif
