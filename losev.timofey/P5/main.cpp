#include<iostream>
namespace losev {
  struct point_t {
    double x, y;
  };
  struct rectangle_t {
    double width, height;
    point_t pos;
  };
  struct Shape {
    virtual double getArea();
    virtual rectangle_t getFrameRect();
  };
}
