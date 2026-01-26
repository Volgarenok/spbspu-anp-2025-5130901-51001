#include <iostream>
#include <cmath>
#include <stdexcept>

namespace shaykhraziev
{
  struct point_t
  {
    double x, y;
  };

  bool operator==(point_t lhs, point_t rhs);
  bool operator!=(point_t lhs, point_t rhs);

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  bool operator==(const rectangle_t& lhs, const rectangle_t& rhs);
  bool operator!=(const rectangle_t& lhs, const rectangle_t& rhs);

  struct Shape
  {
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t to) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };

  struct Rectangle final: Shape
  {
    explicit Rectangle(point_t a, point_t b);
    explicit Rectangle(rectangle_t r);

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

  private:
    point_t center_;
    double width_, height_;
  };

  struct Polygon final: Shape
  {
    explicit Polygon(const point_t* a, size_t size);
    Polygon(const Polygon&) = delete;
    Polygon(Polygon&&) = delete;
    ~Polygon() override;

    Polygon& operator=(const Polygon&) = delete;
    Polygon& operator=(Polygon&&) = delete;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

  private:
    point_t* points_;
    size_t size_;
    point_t center_;
  };

  struct Concave final: Shape
  {
    explicit Concave(const point_t* a, size_t size);

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

  private:
    point_t points_[4];
    point_t center_;
  };

  void scaleRelative(Shape& shp, point_t pt, double coef);
  rectangle_t getAllShapesFrameRect(const Shape* const* shps, size_t size);
  void removeArray(Shape* const* shps, size_t size);
  void outputParams(std::ostream& out, const Shape* const* shps, size_t size);
  double distToLine(point_t d1, point_t d2, point_t m);
  double euclidDist(point_t d1, point_t d2);
  double calcPolygonArea(const point_t* pts, size_t size);
  point_t calcPolygonCentroid(const point_t* pts, size_t size);
}

bool shaykhraziev::operator==(const point_t lhs, const point_t rhs)
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool shaykhraziev::operator!=(const point_t lhs, const point_t rhs)
{
  return !(rhs == lhs);
}

bool shaykhraziev::operator==(const rectangle_t& lhs, const rectangle_t& rhs)
{
  return lhs.width == rhs.width && lhs.height == rhs.height && lhs.pos == rhs.pos;
}

bool shaykhraziev::operator!=(const rectangle_t& lhs, const rectangle_t& rhs)
{
  return !(rhs == lhs);
}

shaykhraziev::Rectangle::Rectangle(point_t a, point_t b):
  center_({a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2}),
  width_(b.x - a.x),
  height_(b.y - a.y)
{}

shaykhraziev::Rectangle::Rectangle(rectangle_t r):
  center_(r.pos),
  width_(r.width),
  height_(r.height)
{}

double shaykhraziev::Rectangle::getArea() const
{
  return width_ * height_;
}

shaykhraziev::rectangle_t shaykhraziev::Rectangle::getFrameRect() const
{
  return {width_, height_, center_};
}

void shaykhraziev::Rectangle::move(point_t to)
{
  center_ = to;
}

void shaykhraziev::Rectangle::move(double dx, double dy)
{
  center_.x += dx;
  center_.y += dy;
}

void shaykhraziev::Rectangle::scale(double coef)
{
  width_ *= coef;
  height_ *= coef;
}

void shaykhraziev::scaleRelative(Shape& shp, point_t pt, double coef)
{
  if (coef == 1) {
    return;
  }

  point_t c = shp.getFrameRect().pos;
  double x = pt.x + (c.x - pt.x) * coef;
  double y = pt.y + (c.y - pt.y) * coef;

  shp.scale(coef);
  shp.move({x, y});
}

shaykhraziev::rectangle_t shaykhraziev::getAllShapesFrameRect (const Shape* const* shps, size_t size)
{
  if (size == 0) {
    return {0, 0, {0, 0}};
  }

  rectangle_t r = shps[0]->getFrameRect();
  double minx = r.pos.x - r.width / 2;
  double miny = r.pos.y - r.height / 2;
  double maxx = r.pos.x + r.width / 2;
  double maxy = r.pos.y + r.height / 2;

  for (size_t i = 1; i < size; i++) {
    r = shps[i]->getFrameRect();
    minx = std::min(minx, r.pos.x - r.width / 2);
    miny = std::min(miny, r.pos.y - r.height / 2);
    maxx = std::max(maxx, r.pos.x + r.width / 2);
    maxy = std::max(maxy, r.pos.y + r.height / 2);
  }

  double width = maxx - minx;
  double height = maxy - miny;
  point_t c = {minx + width / 2, miny + height / 2};

  return {width, height, c};
}

void shaykhraziev::removeArray(Shape* const * shps, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    delete shps[i];
  }

  delete [] shps;
}

void shaykhraziev::outputParams(std::ostream& out, const Shape* const* shps, size_t size)
{
  double area = 0;

  for (size_t i = 0; i < size; i++) {
    double x = shps[i]->getArea();
    area += x;
    out << x << "\n";
  }
  out << area << "\n";

  for (size_t i = 0; i < size; i++) {
    rectangle_t r = shps[i]->getFrameRect();
    out << "(" << r.pos.x << ", " << r.pos.y << ") " << r.height << " " << r.width << "\n";
  }

  rectangle_t all = getAllShapesFrameRect(shps, size);
  out << "(" << all.pos.x << ", " << all.pos.y << ") " << all.width << " " << all.height << "\n";
}

shaykhraziev::Polygon::Polygon(const point_t* a, size_t size):
  points_(new point_t[size]),
  size_(size),
  center_(calcPolygonCentroid(a, size))
{
  if (a == nullptr || size == 0) {
    delete[] points_;
    throw std::invalid_argument("Invalid size");
  }

  for (size_t i = 0; i < size; ++i) {
    points_[i] = a[i];
  }
}

double shaykhraziev::Polygon::getArea() const
{
  return calcPolygonArea(points_, size_);
}

shaykhraziev::rectangle_t shaykhraziev::Polygon::getFrameRect() const
{
  double minx = points_[0].x;
  double miny = points_[0].y;
  double maxx = points_[0].x;
  double maxy = points_[0].y;

  for (size_t i = 1; i < size_; i++) {
    minx = std::min(minx, points_[i].x);
    miny = std::min(miny, points_[i].y);
    maxx = std::max(maxx, points_[i].x);
    maxy = std::max(maxy, points_[i].y);
  }

  double width = maxx - minx;
  double height = maxy - miny;
  point_t p = {minx + width / 2, miny + height / 2};

  return {width, height, p};
}

void shaykhraziev::Polygon::move(double dx, double dy)
{
  for (size_t i = 0; i < size_; i++) {
    points_[i].x += dx;
    points_[i].y += dy;
  }

  center_.x += dx;
  center_.y += dy;
}

void shaykhraziev::Polygon::move(point_t to)
{
  double dx = to.x - center_.x;
  double dy = to.y - center_.y;
  move(dx, dy);
}

void shaykhraziev::Polygon::scale(double coef)
{
  for (size_t i = 0; i < size_; ++i) {
    points_[i].x = center_.x + (points_[i].x - center_.x) * coef;
    points_[i].y = center_.y + (points_[i].y - center_.y) * coef;
  }
}

shaykhraziev::Polygon::~Polygon()
{
  delete[] points_;
}

double shaykhraziev::distToLine(point_t d1, point_t d2, point_t m)
{
  double A = d1.y - d2.y;
  double B = d2.x - d1.x;
  double C = d1.x * d2.y - d1.y * d2.x;

  return (A * m.x + B * m.y + C) / std::sqrt(A * A + B * B);
}

shaykhraziev::Concave::Concave(const point_t* a, size_t size):
  points_(),
  center_()
{
  if (size != 4) {
    throw std::invalid_argument("Must have four vertices");
  }

  double d1 = distToLine(a[0], a[2], a[1]);
  double d3 = distToLine(a[0], a[2], a[3]);
  double d0 = distToLine(a[1], a[3], a[0]);
  double d2 = distToLine(a[1], a[3], a[2]);

  if ((d1 == 0.0 && d3 == 0.0) || round(d1 / std::abs(d1)) == round(d3 / std::abs(d3))) {
    if (std::abs(d1) < std::abs(d3)) {
      center_ = a[1];
    } else {
      center_ = a[3];
    }
  } else if ((d0 == 0.0 && d2 == 0.0) || round(d0 / std::abs(d0)) == round(d2 / std::abs(d2))) {
    if (std::abs(d0) < std::abs(d2)) {
      center_ = a[0];
    } else {
      center_ = a[2];
    }
  } else {
    throw std::invalid_argument("No such concave");
  }

  for (size_t i = 0; i < size; i++) {
    points_[i] = a[i];
  }
}

double shaykhraziev::euclidDist(point_t d1, point_t d2)
{
  return std::sqrt((d1.x - d2.x) * (d1.x - d2.x) + (d1.y - d2.y) * (d1.y - d2.y));
}

double shaykhraziev::calcPolygonArea(const point_t* pts, size_t size)
{
  if (size == 0) {
    return 0;
  }

  if (size < 3) {
    return 0.0;
  }

  double crossSum = 0.0;

  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1 == size) ? 0 : i + 1;
    size_t k = (i == 0) ? size - 1 : i - 1;
    crossSum += pts[i].x * (pts[j].y - pts[k].y);
  }

  return std::abs(crossSum) / 2.0;
}

shaykhraziev::point_t shaykhraziev::calcPolygonCentroid(const point_t* pts, size_t size)
{
  if (size == 0) {
    return {0.0, 0.0};
  }

  if (size == 1) {
    return pts[0];
  }
  if (size == 2) {
    return {(pts[0].x + pts[1].x) / 2, (pts[0].y + pts[1].y) / 2};
  }

  double crossSum = 0.0;
  double cx = 0.0;
  double cy = 0.0;

  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1 == size) ? 0 : i + 1;
    double cross = pts[i].x * pts[j].y - pts[j].x * pts[i].y;
    crossSum += cross;
    cx += (pts[i].x + pts[j].x) * cross;
    cy += (pts[i].y + pts[j].y) * cross;
  }

  double area = std::abs(crossSum) / 2.0;
  cx /= (6 * area);
  cy /= (6 * area);

  return {cx, cy};
}

double shaykhraziev::Concave::getArea() const
{
  double d1 = euclidDist(points_[0], points_[2]);
  double d2 = euclidDist(points_[1], points_[3]);

  double xx = (points_[0].x - points_[2].x) * (points_[1].x - points_[3].x);
  double yy = (points_[0].y - points_[2].y) * (points_[1].y - points_[3].y);

  double cosa = std::abs(xx + yy) / (d1 * d2);
  double sina = std::sqrt(1 - cosa * cosa);

  return 0.5 * d1 * d2 * sina;
}

shaykhraziev::rectangle_t shaykhraziev::Concave::getFrameRect() const
{
  double minx = points_[0].x;
  double miny = points_[0].y;
  double maxx = points_[0].x;
  double maxy = points_[0].y;

  for (size_t i = 1; i < 4; i++) {
    minx = std::min(minx, points_[i].x);
    miny = std::min(miny, points_[i].y);
    maxx = std::max(maxx, points_[i].x);
    maxy = std::max(maxy, points_[i].y);
  }

  double width = maxx - minx;
  double height = maxy - miny;
  point_t p = {minx + width / 2, miny + height / 2};

  return {width, height, p};
}

void shaykhraziev::Concave::move(double dx, double dy)
{
  for (size_t i = 0; i < 4; i++) {
    points_[i].x += dx;
    points_[i].y += dy;
  }

  center_.x += dx;
  center_.y += dy;
}

void shaykhraziev::Concave::move(point_t to)
{
  double dx = to.x - center_.x;
  double dy = to.y - center_.y;
  move(dx, dy);
}

void shaykhraziev::Concave::scale(double coef)
{
  for (size_t i = 0; i < 4; i++) {
    points_[i].x = center_.x + (points_[i].x - center_.x) * coef;
    points_[i].y = center_.y + (points_[i].y - center_.y) * coef;
  }
}

int main()
{
  using namespace shaykhraziev;

  int err = 0;
  size_t shp_cnt = 4;
  Shape** shps = new Shape*[shp_cnt];

  try {
    shps[0] = new Rectangle({0, 0}, {10, 10});
    shps[1] = new Rectangle({3, 3, {-4, -6}});

    point_t pts[] = {{0, 1}, {4, 6}, {-1, -10}, {-5, -5}};
    shps[2] = new Polygon(pts, 4);

    point_t pts2[] = {{0, 5}, {5, 1}, {1, 0}, {2, 2}};
    shps[3] = new Concave(pts2, 4);
  } catch (...) {
    std::cerr << "memalloc error" << "\n";
    err = 1;
  }

  if (err == 0) {
    outputParams(std::cout, shps, shp_cnt);

    double x = 0;
    double y = 0;
    double scale = 0;

    if (std::cin >> x >> y >> scale && scale >= 0) {
      for (size_t i = 0; i < shp_cnt; i++) {
        scaleRelative(*shps[i], {x, y}, scale);
      }
      outputParams(std::cout, shps, shp_cnt);
    } else {
      std::cerr << "bad input" << "\n";
      err = 1;
    }
  }

  removeArray(shps, shp_cnt);

  return err;
}
