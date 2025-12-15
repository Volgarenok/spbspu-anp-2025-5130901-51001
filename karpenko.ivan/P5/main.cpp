#include <iostream>

namespace karpenko
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  struct Shape
  {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(const point_t& point) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coefficient) = 0;
    virtual ~Shape() = default;
  };

  struct Rectangle : Shape
  {
    double width;
    double height;
    point_t center;

    Rectangle(double width, double height, const point_t& center) :
      width(width),
      height(height),
      center(center)
    {
    }
  };
}

int main()
{
  return 0;
}
