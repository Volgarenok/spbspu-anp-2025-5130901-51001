#include <iostream>
#include <cmath>
#include <algorithm>

namespace hachaturyanov {
  struct point_t {
    double x, y;
  };

  bool operator==(point_t a, point_t b);
  bool operator!=(point_t a, point_t b);
  point_t& operator*=(point_t &pnt, double k);
  point_t operator*(point_t pnt, double k);
  point_t& operator+=(point_t &pnt, double k);
  point_t& operator+=(point_t &left, const point_t &right);
  point_t operator+(point_t left, const point_t &right);
  point_t operator-=(point_t &left, const point_t &right);
  point_t operator-(point_t left, const point_t &right);

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
    point_t vertices[4];
    double diag1, diag2;
    point_t pos;
  };

  struct Polygon: Shape {
    explicit Polygon(point_t* pts, size_t npts);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t pnt) override;
    void move(double xsh, double ysh) override;
    void scale(double k) override;
    ~Polygon();
    point_t* points;
    point_t pos;
    size_t npoints;
  };

  double triangleArea(const point_t v1, const point_t v2, const point_t v3);
  double getSumArea(Shape** shs, size_t n);
  rectangle_t getFrame(Shape** shs, size_t n);
}

int main()
{
  using namespace hachaturyanov;
  Shape* shapes[2] = {};
  point_t* polygon = nullptr;

  try {
    shapes[0] = new Complexquad(4, 9, {0, 0});

    point_t* polygon = new point_t[5];
    polygon[0] = {10, 10};
    polygon[1] = {12, 8};
    polygon[2] = {12, 6};
    polygon[3] = {8, 4};
    polygon[4] = {6, 8};
    shapes[1] = new Polygon(polygon, 5);

    try {
      rectangle_t rect = getFrame(shapes, 2);
    } catch (...) {
      !!!!!
    }

  } catch (...) {
    std::cerr << "Error: bad allocation\n";
    delete[] shapes[0];
    delete[] polygon;
    delete[] shapes[1];
    return 1;
  }

  delete[] shapes[0];
  delete[] polygon;
  delete[] shapes[1];
  return 0;
}

hachaturyanov::rectangle_t hachaturyanov::getFrame(Shape** shs, size_t n) {
  rectangle_t* rects = new rectangle_t[n];
  rectangle_t min_rect = shs[0]->getFrameRect();
  rectangle_t max_rect = shs[0]->getFrameRect();

  for (size_t i = 0; i < n; i++) {
    rectangle_t frame_rect = shs[i]->getFrameRect();
    if (frame_rect.pos.x < min_rect.pos.x && frame_rect.pos.y < min_rect.pos.y) {
      min_rect = frame_rect;
    }
    if (frame_rect.pos.x > max_rect.pos.x && frame_rect.pos.y > max_rect.pos.y) {
      max_rect = frame_rect;
    }
  }
  rectangle_t res = {};
  res.pos.x = (min_rect.pos.x + max_rect.pos.x) / 2;
  res.pos.y = (min_rect.pos.y + max_rect.pos.y) / 2;


  delete[] rects;
}

double hachaturyanov::triangleArea(const point_t v1, const point_t v2, const point_t v3)
{
  return 0.5 * std::abs(v1.x * (v2.y - v3.y) + v2.x * (v3.y - v1.y) + v3.x * (v1.y - v2.y));
}

hachaturyanov::Polygon::Polygon(point_t* pts, size_t npts):
 points(nullptr),
 pos(point_t{}),
 npoints(npts)
{
  if (npts > 2) {
    points = new point_t[npoints];
    for (size_t i = 0; i < npoints; i++) {
      points[i] = pts[i];
    }
    point_t sumareasw = {};
    double sumareas = 0;
    for (size_t i = 1; i < npoints - 1; i++) {
      double iarea = triangleArea(points[0], points[i], points[i + 1]);
      double icenterx = (points[0].x + points[i].x + points[i + 1].x) / 3;
      double icentery = (points[0].y + points[i].y + points[i + 1].y) / 3;

      sumareasw += point_t{icenterx, icentery} * iarea;
      sumareas += iarea;
    }
    pos = sumareasw * (1.0 / sumareas);
  } else {
    throw std::logic_error("polygon should have more than 2 vertices");
  }
}

hachaturyanov::Polygon::~Polygon() {
  delete[] points;
}

double hachaturyanov::Polygon::getArea() const {
  double res = 0;
  for (size_t i = 0; i < npoints; i++) {
    if (i != npoints - 1) {
      res += points[i].x * points[i + 1].y - points[i].y * points[i + 1].x;
    } else {
      res += points[i].x * points[0].y - points[i].y * points[0].x;
    }
  }
  return 0.5 * std::abs(res);
}

hachaturyanov::rectangle_t hachaturyanov::Polygon::getFrameRect() const {
  double minx = points[0].x;
  double maxx = points[0].x;
  double miny = points[0].y;
  double maxy = points[0].y;
  for (size_t i = 1; i < npoints; i++) {
    minx = std::min(minx, points[i].x);
    maxx = std::max(maxx, points[i].x);
    miny = std::min(miny, points[i].y);
    maxy = std::max(maxy, points[i].y);
  }
  return rectangle_t{maxx - minx, maxy - miny, point_t{(maxx + minx) / 2, (maxy + miny) / 2}};
}

void hachaturyanov::Polygon::move(point_t pnt) {
  point_t shift = point_t{pnt.x - pos.x, pnt.y - pos.y};
  for (size_t i = 0; i < npoints; i++) {
    points[i] += shift;
  }
  pos = pnt;
}

void hachaturyanov::Polygon::move(double xsh, double ysh) {
  point_t shift = point_t{xsh, ysh};
  pos += shift;
  for (size_t i = 0; i < npoints; i++) {
    points[i] += shift;
  }
}

void hachaturyanov::Polygon::scale(double k) {
  if (k > 0) {
    for (size_t i = 0; i < npoints; i++) {
      point_t rel = points[i] - pos;
      points[i] = (pos + rel * k);
    }
  } else {
    throw std::logic_error("coefficient must be positive");
  }
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

hachaturyanov::rectangle_t hachaturyanov::Complexquad::getFrameRect() const {
  double minx = std::min({vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x});
  double maxx = std::max({vertices[0].x, vertices[1].x, vertices[2].x, vertices[3].x});
  double miny = std::min({vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y});
  double maxy = std::max({vertices[0].y, vertices[1].y, vertices[2].y, vertices[3].y});
  return rectangle_t{maxx - minx, maxy - miny, point_t{(minx + maxx), (miny + maxy)} * 0.5};
}

void hachaturyanov::Complexquad::move(point_t pnt) {
  point_t shift = {pnt.x - pos.x, pnt.y - pos.y};
  for (size_t i = 0; i < 4; i++) {
    vertices[i] += shift;
  }
  pos = pnt;
}

void hachaturyanov::Complexquad::move(double xsh, double ysh) {
  point_t shift = {xsh, ysh};
  for (size_t i = 0; i < 4; i++) {
    vertices[i] += shift;
  }
  pos += shift;
}

void hachaturyanov::Complexquad::scale(double k) {
  if (k > 0) {
    diag1 *= k;
    diag2 *= k;
    for (size_t i = 0; i < 4; i++) {
      point_t rel = vertices[i] - pos;
      vertices[i] = (pos + rel * k);
    }
  } else {
    throw std::logic_error("coefficient must be positive");
  }
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

bool hachaturyanov::operator==(point_t a, point_t b) {
  return a.x == b.x && a.y == b.y;
}

bool hachaturyanov::operator!=(point_t a, point_t b) {
  return !(a==b);
}

hachaturyanov::point_t& hachaturyanov::operator*=(point_t &pnt, double k) {
  pnt.x *= k;
  pnt.y *= k;
  return pnt;
}

hachaturyanov::point_t hachaturyanov::operator*(point_t pnt, double k) {
  return pnt *= k;
}

hachaturyanov::point_t& hachaturyanov::operator+=(point_t &pnt, double k) {
  pnt.x += k;
  pnt.y += k;
  return pnt;
}

hachaturyanov::point_t& hachaturyanov::operator+=(point_t &left, const point_t &right) {
  left.x += right.x;
  left.y += right.y;
  return left;
}

hachaturyanov::point_t hachaturyanov::operator+(point_t left, const point_t &right) {
  return left += right;
}

hachaturyanov::point_t hachaturyanov::operator-=(point_t &left, const point_t &right) {
  left.x -= right.x;
  left.y -= right.y;
}

hachaturyanov::point_t hachaturyanov::operator-(point_t left, const point_t &right) {
  return left -= right;
}
