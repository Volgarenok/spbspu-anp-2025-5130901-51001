#ifndef SHAPE_HPP
#define SHAPE_HPP
namespace kitserov
{
  struct point_t
  {
    float x, y;
  };

  struct rectangle_t
  {
    float width, height;
    point_t pos;
  };

  struct Shape
  {
    virtual ~Shape() = default;
    virtual float getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(float dx, float dy) noexcept = 0;
    virtual void move(const point_t& p) noexcept = 0;
    virtual void scale(float k) = 0 noexcept = 0;
  };
}
#endif
