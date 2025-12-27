#include <iostream>
#include <__xlocale.h>

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

  struct Polygon : Shape {
    point_t * points;
    size_t size;
    point_t center;
    double area;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    explicit Polygon(point_t * a, size_t size);
  };

  struct Concave : Shape {
    point_t points[4];
    point_t center;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    explicit Concave(const point_t * a, size_t size);
  };

  void scaleRelative(Shape& shp, point_t pt, double coef);
  rectangle_t getAllShapesFrameRect(Shape** shps, size_t size);
  void removeArray(Shape**shps, size_t size);
  void printParams(Shape** shps, size_t size);
  double distToLine(point_t d1, point_t d2, point_t m);
  double euclidDist(point_t d1, point_t d2);
}

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

draw::Rectangle::Rectangle(point_t a, point_t b): center() {
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
  if (coef == 1) return;
  point_t c = shp.getFrameRect().pos;
  double x = pt.x + (c.x - pt.x)*coef;
  double y = pt.y + (c.y - pt.y)*coef;
  shp.scale(coef);
  shp.move({x, y});
}

draw::rectangle_t draw::getAllShapesFrameRect(Shape **shps, size_t size) {
  if (size == 0) return {0,0,{0,0}};

  rectangle_t r = shps[0]->getFrameRect();
  double minx = r.pos.x - r.width/2,
  miny = r.pos.y - r.height/2,
  maxx = r.pos.x + r.width/2,
  maxy = r.pos.y + r.height/2;
  for (size_t i = 1; i < size; i++) {
    r = shps[i]->getFrameRect();
    minx = std::min(minx, r.pos.x - r.width/2);
    miny = std::min(miny, r.pos.y - r.height/2);
    maxx = std::max(maxx, r.pos.x + r.width/2);
    maxy = std::max(maxy, r.pos.y + r.height/2);
  }

  double width = maxx - minx;
  double height = maxy - miny;
  point_t c = {minx + width/2, miny + height/2};

  return {width, height, c};
}

void draw::removeArray(Shape**shps, size_t size) {
  for (size_t i = 0; i < size; i++) {
    delete shps[i];
  }
}

void draw::printParams(Shape** shps, size_t size) {
  double area = 0;
  for (size_t i = 0; i < size; i++) {
    double x = shps[i]->getArea();
    area += x;
    std::cout << x << "\n";
  }
  std::cout << area << "\n";

  for (size_t i = 0; i < size; i++) {
    rectangle_t r = shps[i]->getFrameRect();
    std::cout << "(" << r.pos.x << ", " << r.pos.y << ") " << r.height << " " << r.width << "\n";
  }
  rectangle_t all = getAllShapesFrameRect(shps, size);
  std::cout << "(" << all.pos.x << ", " << all.pos.y << ") " << all.width << " " << all.height << "\n";
}

draw::Polygon::Polygon(point_t *a, size_t size): points(a), size(size), center({0, 0}){
  if (size == 0) throw std::invalid_argument("Invalid size");
  double x = 0, y = 0, S = 0;

  if (size == 1) {
    x = a[0].x;
    y = a[0].y;
  }
  else if (size == 2) {
    x = (a[0].x + a[1].x) / 2;
    y = (a[0].y + a[1].y) / 2;
  }
  else {
    for (size_t i = 0; i < size; i++) {
      size_t j = i + 1, k = i - 1;
      if (j == size) j = 0;
      if (i == 0) k = size - 1;
      S += a[i].x * (a[j].y - a[k].y);
      x += (a[i].x + a[j].x) * (a[i].x * a[j].y - a[j].x * a[i].y);
      y += (a[i].y + a[j].y) * (a[i].x * a[j].y - a[j].x * a[i].y);
    }
    S = abs(S) / 2.0;
    x = x/(6*S);
    y = y/(6*S);
    area = S;
  }

  center = {x, y};
}

double draw::Polygon::getArea() const {
  return area;
}

draw::rectangle_t draw::Polygon::getFrameRect() const {
  double minx = points[0].x;
  double miny = points[0].y;
  double maxx = points[0].x;
  double maxy = points[0].y;
  for (size_t i = 1; i < size; i++) {
    minx = std::min(minx, points[i].x);
    miny = std::min(miny, points[i].y);
    maxx = std::max(maxx, points[i].x);
    maxy = std::max(maxy, points[i].y);
  }
  double width = maxx - minx, height = maxy - miny;
  point_t p = {minx + width/2, miny + height/2};

  return {width, height, p};
}

void draw::Polygon::move(double dx, double dy) {
  for (size_t i = 0; i < size; i++) {
    points[i].x += dx;
    points[i].y += dy;
  }
}

void draw::Polygon::move(point_t to) {
  double dx = to.x - center.x, dy = to.y - center.y;
  move(dx, dy);
}

void draw::Polygon::scale(double coef) {
  area *= coef * coef;
  for (size_t i = 0; i < size; ++i) {
    points[i].x = center.x + (points[i].x - center.x) * coef;
    points[i].y = center.y + (points[i].y - center.y) * coef;
  }
}

double draw::distToLine(point_t d1, point_t d2, point_t m) {
  double A = d1.y - d2.y;
  double B = d2.x - d1.x;
  double C = d1.x * d2.y - d1.y * d2.x;

  return (A*m.x + B*m.y + C) / sqrt(A*A + B*B);
}


draw::Concave::Concave(const point_t *a, size_t size): points(), center() {
  if (size != 4) throw std::invalid_argument("Must have four vertices");
  // 0 - 2
  double d1 = distToLine(a[0], a[2], a[1]);
  double d3 = distToLine(a[0], a[2], a[3]);

  // 1 - 3
  double d0 = distToLine(a[1], a[3], a[0]);
  double d2 = distToLine(a[1], a[3], a[2]);

  if (d1/abs(d1) == d3/abs(d3)) {
    if (abs(d1) < abs(d3)) {
      center = a[1];
    } else {
      center = a[3];
    }
  } else if (d0/abs(d0) == d2/abs(d2)) {
    if (abs(d0) < abs(d2)) {
      center = a[0];
    } else {
      center = a[2];
    }
  } else {
    throw std::invalid_argument("No such concave");
  }

  for (size_t i = 0; i < size; i++) {
    points[i] = a[i];
  }
}

double draw::euclidDist(point_t d1, point_t d2) {
  return sqrt((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y));
}


double draw::Concave::getArea() const {
  double d1 = euclidDist(points[0], points[2]);
  double d2 = euclidDist(points[1], points[3]);

  double xx = (points[0].x - points[2].x) * (points[1].x - points[3].x);
  double yy = (points[0].y - points[2].y) * (points[1].y - points[3].y);

  double cosa = abs(xx+yy) / (d1+d2);
  double sina = sqrt(1 - cosa*cosa);

  return 0.5 * d1 * d2 * sina;
}

draw::rectangle_t draw::Concave::getFrameRect() const {
  double minx = points[0].x;
  double miny = points[0].y;
  double maxx = points[0].x;
  double maxy = points[0].y;
  for (auto point : points) {
    minx = std::min(minx, point.x);
    miny = std::min(miny, point.y);
    maxx = std::max(maxx, point.x);
    maxy = std::max(maxy, point.y);
  }
  double width = maxx - minx, height = maxy - miny;
  point_t p = {minx + width/2, miny + height/2};

  return {width, height, p};
}

void draw::Concave::move(double dx, double dy) {
  for (auto & point : points) {
    point.x += dx;
    point.y += dy;
  }
  center.x += dx;
  center.y += dy;
}

void draw::Concave::move(point_t to) {
  double dx = to.x - center.x, dy = to.y - center.y;
  move(dx, dy);
}

void draw::Concave::scale(double coef) {
  for (auto & point : points) {
    point.x = center.x + (point.x - center.x) * coef;
    point.y = center.y + (point.y - center.y) * coef;
  }
}


int main() {
  using namespace draw;
  int err = 0;
  size_t shp_cnt = 4;
  Shape *shps[4] = {};
  try {
    shps[0] = new Rectangle({0, 0}, {10, 10});
    shps[1] = new Rectangle({3, 3, {-4, -6}});

    point_t pts[4] = {{0,1}, {4, 6}, {-1, -10}, {-5, -5}};
    shps[2] = new Polygon(pts, 4);

    point_t pts2[4] = {{0,5}, {5,1}, {1,0}, {2,2}};
    shps[3] = new Concave(pts2, 4);

  } catch (...) {
    std::cerr << "memalloc error" << "\n";
    err = 1;
  }

  printParams(shps, shp_cnt);

  double x = 0, y = 0, scale = 0;
  if (std::cin >> x >> y >> scale && scale >= 0) {
    for (size_t i = 0; i < shp_cnt; i++) {
      scaleRelative(*shps[i], {x, y}, scale);
    }
    printParams(shps, shp_cnt);
  } else {
    std::cerr << "bad input" << "\n";
    err = 1;
  }

  removeArray(shps, shp_cnt);

  return err;
}
