#include <iostream>

namespace hachaturyanov {
  struct point_t {
    int x, y;
  };

  struct rectangle_t {
    size_t width, height;
    int pos;
  };

  struct Shape {
    virtual size_t getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t pnt) = 0;
    virtual void move(int xsh, int ysh) = 0;
    virtual void scale(double k) = 0;
    virtual ~Shape() = default;
  };
}

int main()
{

}
