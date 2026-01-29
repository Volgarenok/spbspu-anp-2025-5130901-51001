#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace alekseev
{
  struct point_t
  {
    double x;
    double y;
  };

  bool operator==(const point_t &lhs, const point_t &rhs) noexcept;
  bool operator!=(const point_t &lhs, const point_t &rhs) noexcept;

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  bool operator==(const rectangle_t &lhs, const rectangle_t &rhs) noexcept;
  bool operator!=(const rectangle_t &lhs, const rectangle_t &rhs) noexcept;

  struct Shape
  {
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    void scale(double coef);
    void scaleUnchecked(double coef);
    virtual void move(const point_t &to) = 0;
    virtual void move(double dx, double dy) = 0;

  protected:
    virtual void doScale(double coef) = 0;
  };

  class Rectangle final: public Shape
  {
  public:
    Rectangle(const point_t &a, const point_t &b);
    explicit Rectangle(const rectangle_t &r);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &to) override;
    void move(double dx, double dy) override;

  private:
    point_t center_;
    double width_;
    double height_;

    void doScale(double coef) override;
  };

  class Square final: public Shape
  {
  public:
    Square(const point_t &center, double side);
    explicit Square(const rectangle_t &r);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &to) override;
    void move(double dx, double dy) override;

  private:
    point_t center_;
    double side_;

    void doScale(double coef) override;
  };

  class Xquare final: public Shape
  {
  public:
    Xquare(const point_t &center, double diagonal);
    explicit Xquare(const rectangle_t &r);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t &to) override;
    void move(double dx, double dy) override;

  private:
    point_t center_;
    double diagonal_;

    void doScale(double coef) override;
  };

  void scaleRelative(Shape &shp, const point_t &about, double coef);
  void scaleAllRelative(Shape *const *shps, size_t size, const point_t &about, double coef);
  rectangle_t getAllShapesFrameRect(const Shape *const *shps, size_t size);
  void outputParams(std::ostream &out, const Shape *const *shps, size_t size);
  void removeArray(Shape **shps, size_t size);
}

bool alekseev::operator==(const point_t &lhs, const point_t &rhs) noexcept
{
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool alekseev::operator!=(const point_t &lhs, const point_t &rhs) noexcept
{
  return !(lhs == rhs);
}

bool alekseev::operator==(const rectangle_t &lhs, const rectangle_t &rhs) noexcept
{
  return lhs.width == rhs.width && lhs.height == rhs.height && lhs.pos == rhs.pos;
}

bool alekseev::operator!=(const rectangle_t &lhs, const rectangle_t &rhs) noexcept
{
  return !(lhs == rhs);
}

void alekseev::Shape::scale(double coef)
{
  if (!std::isfinite(coef) || coef <= 0.0)
  {
    throw std::invalid_argument("scale: bad coef");
  }
  if (coef == 1.0)
  {
    return;
  }
  doScale(coef);
}

void alekseev::Shape::scaleUnchecked(double coef)
{
  doScale(coef);
}

alekseev::Rectangle::Rectangle(const point_t &a, const point_t &b):
  center_({(a.x + b.x) / 2.0, (a.y + b.y) / 2.0}),
  width_(std::abs(b.x - a.x)),
  height_(std::abs(b.y - a.y))
{
  if (!std::isfinite(a.x) || !std::isfinite(a.y) || !std::isfinite(b.x) || !std::isfinite(b.y)) {
    throw std::invalid_argument("Rectangle: bad points");
  }
  if (!std::isfinite(width_) || !std::isfinite(height_) || width_ <= 0.0 || height_ <= 0.0) {
    throw std::invalid_argument("Rectangle: bad size");
  }
}

alekseev::Rectangle::Rectangle(const rectangle_t &r):
  center_(r.pos),
  width_(r.width),
  height_(r.height)
{
  if (!std::isfinite(center_.x) || !std::isfinite(center_.y)) {
    throw std::invalid_argument("Rectangle: bad center");
  }
  if (!std::isfinite(width_) || !std::isfinite(height_) || width_ <= 0.0 || height_ <= 0.0) {
    throw std::invalid_argument("Rectangle: bad size");
  }
}

double alekseev::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

alekseev::rectangle_t alekseev::Rectangle::getFrameRect() const noexcept
{
  return {width_, height_, center_};
}

void alekseev::Rectangle::move(const point_t &to)
{
  if (!std::isfinite(to.x) || !std::isfinite(to.y)) {
    throw std::invalid_argument("move: bad point");
  }
  center_ = to;
}

void alekseev::Rectangle::move(double dx, double dy)
{
  if (!std::isfinite(dx) || !std::isfinite(dy)) {
    throw std::invalid_argument("move: bad shift");
  }
  center_.x += dx;
  center_.y += dy;
}

void alekseev::Rectangle::doScale(double coef)
{
  width_ *= coef;
  height_ *= coef;
}

alekseev::Square::Square(const point_t &center, double side):
  center_(center),
  side_(side)
{
  if (!std::isfinite(center_.x) || !std::isfinite(center_.y)) {
    throw std::invalid_argument("Square: bad center");
  }
  if (!std::isfinite(side_) || side_ <= 0.0) {
    throw std::invalid_argument("Square: bad side");
  }
}

alekseev::Square::Square(const rectangle_t &r):
  center_(r.pos),
  side_(r.width)
{
  if (!std::isfinite(center_.x) || !std::isfinite(center_.y)) {
    throw std::invalid_argument("Square: bad center");
  }
  if (!std::isfinite(r.width) || !std::isfinite(r.height) || r.width <= 0.0 || r.height <= 0.0) {
    throw std::invalid_argument("Square: bad size");
  }
  if (r.width != r.height) {
    throw std::invalid_argument("Square: not a square");
  }
}

double alekseev::Square::getArea() const noexcept
{
  return side_ * side_;
}

alekseev::rectangle_t alekseev::Square::getFrameRect() const noexcept
{
  return {side_, side_, center_};
}

void alekseev::Square::move(const point_t &to)
{
  if (!std::isfinite(to.x) || !std::isfinite(to.y)) {
    throw std::invalid_argument("move: bad point");
  }
  center_ = to;
}

void alekseev::Square::move(double dx, double dy)
{
  if (!std::isfinite(dx) || !std::isfinite(dy)) {
    throw std::invalid_argument("move: bad shift");
  }
  center_.x += dx;
  center_.y += dy;
}

void alekseev::Square::doScale(double coef)
{
  side_ *= coef;
}

alekseev::Xquare::Xquare(const point_t &center, double diagonal):
  center_(center),
  diagonal_(diagonal)
{
  if (!std::isfinite(center_.x) || !std::isfinite(center_.y)) {
    throw std::invalid_argument("Xquare: bad center");
  }
  if (!std::isfinite(diagonal_) || diagonal_ <= 0.0) {
    throw std::invalid_argument("Xquare: bad diagonal");
  }
}

alekseev::Xquare::Xquare(const rectangle_t &r):
  center_(r.pos),
  diagonal_(r.width)
{
  if (!std::isfinite(center_.x) || !std::isfinite(center_.y)) {
    throw std::invalid_argument("Xquare: bad center");
  }
  if (!std::isfinite(r.width) || !std::isfinite(r.height) || r.width <= 0.0 || r.height <= 0.0) {
    throw std::invalid_argument("Xquare: bad size");
  }
  if (r.width != r.height) {
    throw std::invalid_argument("Xquare: bad frame");
  }
}

double alekseev::Xquare::getArea() const noexcept
{
  return (diagonal_ * diagonal_) / 2.0;
}

alekseev::rectangle_t alekseev::Xquare::getFrameRect() const noexcept
{
  return {diagonal_, diagonal_, center_};
}

void alekseev::Xquare::move(const point_t &to)
{
  if (!std::isfinite(to.x) || !std::isfinite(to.y)) {
    throw std::invalid_argument("move: bad point");
  }
  center_ = to;
}

void alekseev::Xquare::move(double dx, double dy)
{
  if (!std::isfinite(dx) || !std::isfinite(dy)) {
    throw std::invalid_argument("move: bad shift");
  }
  center_.x += dx;
  center_.y += dy;
}

void alekseev::Xquare::doScale(double coef)
{
  diagonal_ *= coef;
}

void alekseev::scaleRelative(Shape &shp, const point_t &about, double coef)
{
  const point_t old_center = shp.getFrameRect().pos;
  const double new_x = about.x + (old_center.x - about.x) * coef;
  const double new_y = about.y + (old_center.y - about.y) * coef;
  const point_t new_center = {new_x, new_y};
  shp.scaleUnchecked(coef);
  shp.move(new_center);
}

void alekseev::scaleAllRelative(Shape *const *shps, size_t size, const point_t &about, double coef)
{
  if (!std::isfinite(about.x) || !std::isfinite(about.y)) {
    throw std::invalid_argument("scaleRelative: bad point");
  }
  if (!std::isfinite(coef) || coef <= 0.0) {
    throw std::invalid_argument("scaleRelative: bad coef");
  }
  if (coef == 1.0) {
    return;
  }
  for (size_t i = 0; i < size; ++i) {
    scaleRelative(*shps[i], about, coef);
  }
}

alekseev::rectangle_t alekseev::getAllShapesFrameRect(const Shape *const *shps, size_t size)
{
  if (size == 0) {
    return {0.0, 0.0, {0.0, 0.0}};
  }

  rectangle_t r = shps[0]->getFrameRect();
  double min_x = r.pos.x - r.width / 2.0;
  double max_x = r.pos.x + r.width / 2.0;
  double min_y = r.pos.y - r.height / 2.0;
  double max_y = r.pos.y + r.height / 2.0;

  for (size_t i = 1; i < size; ++i) {
    r = shps[i]->getFrameRect();
    min_x = std::min(min_x, r.pos.x - r.width / 2.0);
    max_x = std::max(max_x, r.pos.x + r.width / 2.0);
    min_y = std::min(min_y, r.pos.y - r.height / 2.0);
    max_y = std::max(max_y, r.pos.y + r.height / 2.0);
  }

  const double width = max_x - min_x;
  const double height = max_y - min_y;
  const point_t pos = {min_x + width / 2.0, min_y + height / 2.0};
  return {width, height, pos};
}

void alekseev::outputParams(std::ostream &out, const Shape *const *shps, size_t size)
{
  double sum_area = 0.0;

  for (size_t i = 0; i < size; ++i) {
    const double area = shps[i]->getArea();
    sum_area += area;
    out << area << "\n";
  }
  out << sum_area << "\n";

  for (size_t i = 0; i < size; ++i) {
    const rectangle_t r = shps[i]->getFrameRect();
    out << "(" << r.pos.x << ", " << r.pos.y << ") "
        << r.height << " " << r.width << "\n";
  }

  const rectangle_t all = getAllShapesFrameRect(shps, size);
  out << "(" << all.pos.x << ", " << all.pos.y << ") "
      << all.height << " " << all.width << "\n";
}

void alekseev::removeArray(Shape **shps, size_t size)
{
  for (size_t i = 0; i < size; ++i) {
    delete shps[i];
    shps[i] = nullptr;
  }
}

int main()
{
  const size_t shape_count = 6;
  alekseev::Shape *shapes[shape_count] = {};

  try {
    shapes[0] = new alekseev::Rectangle({0.0, 0.0}, {10.0, 5.0});
    shapes[1] = new alekseev::Rectangle(alekseev::rectangle_t{4.0, 8.0, {-5.0, 3.0}});
    shapes[2] = new alekseev::Square({2.0, 2.0}, 6.0);
    shapes[3] = new alekseev::Square(alekseev::rectangle_t{5.0, 5.0, {10.0, -2.0}});
    shapes[4] = new alekseev::Xquare({-3.0, -3.0}, 8.0);
    shapes[5] = new alekseev::Xquare(alekseev::rectangle_t{10.0, 10.0, {0.0, 12.0}});
  } catch (const std::exception &e) {
    std::cerr << "init error: " << e.what() << "\n";
    alekseev::removeArray(shapes, shape_count);
    return 2;
  }

  alekseev::outputParams(std::cout, shapes, shape_count);

  alekseev::point_t about = {0.0, 0.0};
  double coef = 0.0;

  if (!(std::cin >> about.x >> about.y >> coef)) {
    std::cerr << "bad input\n";
    alekseev::removeArray(shapes, shape_count);
    return 1;
  }

  try {
    alekseev::scaleAllRelative(shapes, shape_count, about, coef);
    alekseev::outputParams(std::cout, shapes, shape_count);
  } catch (const std::exception &e) {
    std::cerr << "scale error: " << e.what() << "\n";
    alekseev::removeArray(shapes, shape_count);
    return 2;
  }

  alekseev::removeArray(shapes, shape_count);
  return 0;
}
