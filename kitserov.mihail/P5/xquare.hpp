#ifndef XQUARE_HPP
#define XQUARE_HPP

#include "shape.hpp"

namespace kitserov
{
  struct Xquare final: Shape
  {
    Xquare(point_t p, float s) noexcept;
    float getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(float dx, float dy) noexcept override;
    void move(const point_t& p) noexcept override;
  private:
    void doScale(float k) override;
    point_t centre_;
    float side_;
  };
}

#endif
