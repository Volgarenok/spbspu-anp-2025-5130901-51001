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

  struct Complexquad: Shape {
    explicit Complexquad(double sa, double sb, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t pnt) override;
    void move(double xsh, double ysh) override;
    void scale(double k) override;
   private:
    double sidea, sideb;
    point_t pos;
  };
}

int main()
{

}

hachaturyanov::Rectangle::Rectangle(double w, double h, point_t p):
 Shape(),
 width(w),
 height(h),
 pos(p)
{
  if (width <= 0 || height <= 0) {
    throw std::logic_error("width and height must be positive");
  }
}

double hachaturyanov::Rectangle::getArea() const {
  return width * height;
}

hachaturyanov::rectangle_t hachaturyanov::Rectangle::getFrameRect() const {
  return rectangle_t{width, height, pos};
}

void hachaturyanov::Rectangle::move(point_t pnt) {
  pos = pnt;
}

void hachaturyanov::Rectangle::move(double xsh, double ysh) {
  pos.x += xsh;
  pos.y += ysh;
}

void hachaturyanov::Rectangle::scale(double k) {
  if (k > 0) {
    width *= k;
    height *= k;
  } else {
    throw std::logic_error("coefficient must be positive");
  }
}
