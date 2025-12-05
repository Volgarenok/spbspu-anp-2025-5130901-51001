#include <iostream>

namespace draw {
  struct point_t {
    double x, y;
  };

  bool operator==(point_t lhs, point_t rhs);
  bool operator!=(point_t lhs, point_t rhs);

  struct rectangle_t {
    double width, height;
    point_t pos;
  };

  bool operator==(const rectangle_t &lhs, const rectangle_t &rhs);
  bool operator!=(const rectangle_t &lhs, const rectangle_t &rhs);

  struct Shape {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual ~Shape() = default;
    virtual void move(point_t to) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };

  struct Rectangle : Shape {
    point_t center;
    double width, height;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    explicit Rectangle(point_t a, point_t b);
    explicit Rectangle(rectangle_t r);
  };

  void scaleRelative(Shape& shp, point_t pt, double coef);
}

// explicit Rectangle(rectangle_t r) {
//   a = {r.pos.x-r.width/2, r.pos.y-r.height/2};
//   b = {r.pos.x+r.width/2, r.pos.y+r.height/2};
// }

bool draw::operator==(const point_t lhs, const point_t rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool draw::operator!=(const point_t lhs, const point_t rhs) {
  return !(rhs == lhs);
}

bool draw::operator==(const rectangle_t &lhs, const rectangle_t &rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height && lhs.pos == rhs.pos;
}

bool draw::operator!=(const rectangle_t &lhs, const rectangle_t &rhs) {
  return !(rhs == lhs);
}

draw::Rectangle::Rectangle(point_t a, point_t b) {
  center = {a.x + (b.x - a.x)/2, a.y + (b.y - a.y)/2};
  width = b.x - a.x;
  height = b.y - a.y;
}

draw::Rectangle::Rectangle(rectangle_t r):
  center(r.pos), width(r.width), height(r.height) {}

double draw::Rectangle::getArea() const {
  return width * height;
}

draw::rectangle_t draw::Rectangle::getFrameRect() const {
  return {width, height, center};
}

void draw::Rectangle::move(point_t to) {
  center = to;
}

void draw::Rectangle::move(double dx, double dy) {
  center.x += dx;
  center.y += dy;
}

void draw::Rectangle::scale(double coef) {
  width *= coef;
  height *= coef;
}

void draw::scaleRelative(Shape &shp, point_t pt, double coef) {
  point_t c = shp.getFrameRect().pos;
  double x = pt.x + (c.x - pt.x)*coef;
  double y = pt.y + (c.y - pt.y)*coef;
  shp.scale(coef);
  shp.move({x, y});
}






