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
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t pnt) = 0;
    virtual void move(double xsh, double ysh) = 0;
    virtual void scale(double k) = 0;
    virtual ~Shape() = default;
  };

  struct Rectangle: Shape {
    explicit Rectangle(double w, double h, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t pnt) override;
    void move(double xsh, double ysh) override;
    void scale(double k) override;
   private:
    double width, height;
    point_t pos;
  };
}

int main()
{

}

hachaturyanov::Rectangle::Rectangle(double w, double h, point_t p):
 width(w),
 height(h),
 pos(p)
{}
