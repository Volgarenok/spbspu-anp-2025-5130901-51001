#include <iostream>
#include <cmath>

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
    explicit Complexquad(double d1, double d2, point_t p);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t pnt) override;
    void move(double xsh, double ysh) override;
    void scale(double k) override;
   private:
    point_t vertices[4];
    double diag1, diag2;
    point_t pos;
  };

  double triangleArea(const point_t v1, const point_t v2, const point_t v3);
}

int main()
{

}

double hachaturyanov::triangleArea(const point_t v1, const point_t v2, const point_t v3) {
  return 0.5 * std::abs(v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y));
}

hachaturyanov::Complexquad::Complexquad(double d1, double d2, point_t p):
 Shape(),
 vertices({}),
 diag1(d1),
 diag2(d2),
 pos(p)
{
  if (d1 > 0 && d2 > 0) {
    vertices[0] = {p.x - d1 / 2, p.y};
    vertices[1] = {p.x + d1 / 2, p.y};
    vertices[2] = {p.x, p.y - d2 / 2};
    vertices[3] = {p.x, p.y + d2 / 2};
  } else {
    throw std::logic_error("diagonals must be positive");
  }
}

double hachaturyanov::Complexquad::getArea() const {
  double area1 = hachaturyanov::triangleArea(pos, vertices[1], vertices[2]);
  double area2 = hachaturyanov::triangleArea(pos, vertices[0], vertices[3]);
  return area1 + area2;
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
