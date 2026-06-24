#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

namespace kitserov
{
  struct Rectangle final: Shape
  {
    Rectangle(point_t p, float w, float h) noexcept;
    float getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(float dx, float dy) noexcept override;
    void move(const point_t& p) noexcept override;
  private:
    void doScale(float k) override;
    rectangle_t rect_;
  };
}

#endif
