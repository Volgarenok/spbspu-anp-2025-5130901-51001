#include <iostream>

namespace hachaturyanov {
  struct point_t {
    double x, y;
  };

  struct rectangle_t {
    double width, height;
    point_t pos;
  };

  struct Shape {
    virtual size_t getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t pnt) = 0;
    virtual void move(double xsh, double ysh) = 0;
    virtual void scale(double k) = 0;
    virtual ~Shape() = default;
  };


}

int main()
{

}
