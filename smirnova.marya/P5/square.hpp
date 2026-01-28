#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "shape.hpp"

namespace smirnova
{
  class Square final: public Shape
  {
  public:
    Square(double side, const point_t& center);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& p) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double k) noexcept override;

  private:
    double side_;
    point_t center_;
  };
}

#endif

