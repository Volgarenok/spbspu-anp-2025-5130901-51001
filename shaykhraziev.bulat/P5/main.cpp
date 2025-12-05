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
    virtual void move(point_t to) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };

  struct Rectangle : Shape {
    point_t a, b;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    Rectangle(point_t a, point_t b) : a(a), b(b) {}
    Rectangle(rectangle_t r) {
      a = {r.pos.x-r.width/2, r.pos.y-r.height/2};
      b = {r.pos.x+r.width/2, r.pos.y+r.height/2};
    }
  };

}