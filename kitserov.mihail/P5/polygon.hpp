#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "shape.hpp"
#include <cstddef>

namespace kitserov
{
  struct Polygon final: Shape
  {
    Polygon(point_t* vertices, size_t vertexCount);
    ~Polygon() noexcept;
    float getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(float dx, float dy) noexcept override;
    void move(const point_t& p) noexcept override;
  private:
    void doScale(float k) override;
    point_t* vertices_;
    size_t vertexCount_;
    point_t center_;
  };
}

#endif
