#ifndef SHAPE_HPP
#define SHAPE_HPP

namespace smirnova
{
  struct point_t
  {
    double x, y;
  };
  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };
  struct Shape
  {
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& p) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void scale(double k) noexcept = 0;
  };

}

#endif

