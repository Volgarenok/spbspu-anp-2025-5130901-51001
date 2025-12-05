#include <iostream>

namespace draw {
  struct point_t {
    double x, y;
  };

  struct rectangle_t {
    double width, height;
    point_t pos;
  };

  struct Shape {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual ~Shape() = default;
    virtual void move(const point_t& to) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };

}