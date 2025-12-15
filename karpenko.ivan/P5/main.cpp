#include <iostream>
#include <cmath>

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

    double getArea() const override
    {
      return width * height;
    }

    rectangle_t getFrameRect() const override
    {
      rectangle_t result = {width, height, center};
      return result;
    }

    void move(const point_t& point) override
    {
      center = point;
    }

    void move(double dx, double dy) override
    {
      center.x += dx;
      center.y += dy;
    }

    void scale(double coefficient) override
    {
      width *= coefficient;
      height *= coefficient;
    }
  };

  struct Triangle : Shape
  {
    point_t vertexA;
    point_t vertexB;
    point_t vertexC;
    point_t center;

    Triangle(const point_t& a, const point_t& b, const point_t& c) :
      vertexA(a),
      vertexB(b),
      vertexC(c)
    {
    }
  };
}

int main()
{
  return 0;
}
