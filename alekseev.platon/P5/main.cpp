#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace draw
{
  struct point_t
  {
    double x;
    double y;
  };

  bool operator==(point_t lhs, point_t rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  bool operator!=(point_t lhs, point_t rhs)
  {
    return !(lhs == rhs);
  }

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  bool operator==(const rectangle_t &lhs, const rectangle_t &rhs)
  {
    return lhs.width == rhs.width && lhs.height == rhs.height && lhs.pos == rhs.pos;
  }

  bool operator!=(const rectangle_t &lhs, const rectangle_t &rhs)
  {
    return !(lhs == rhs);
  }

  struct Shape
  {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual ~Shape() = default;

    virtual void move(point_t to) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };

  class Rectangle : public Shape
  {
  public:
    Rectangle(point_t a, point_t b);
    explicit Rectangle(rectangle_t r);

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

  private:
    point_t center_;
    double width_;
    double height_;
  };

  class Square : public Shape
  {
  public:
    Square(point_t center, double side);
    explicit Square(rectangle_t r);

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

  private:
    point_t center_;
    double side_;
  };

  class Xquare : public Shape
  {
  public:
    Xquare(point_t center, double diagonal);
    explicit Xquare(rectangle_t r);

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t to) override;
    void move(double dx, double dy) override;
    void scale(double coef) override;

  private:
    point_t center_;
    double diagonal_;
  };

  enum class ReadStatus
  {
    ok,
    bad_input,
    bad_coef
  };

  void scaleRelative(Shape &shp, point_t about, double coef);
  void scaleAllRelative(Shape *const *shps, size_t size, point_t about, double coef);
  rectangle_t getAllShapesFrameRect(Shape *const *shps, size_t size);
  void outputParams(std::ostream &out, Shape *const *shps, size_t size);
  void removeArray(Shape **shps, size_t size);
  ReadStatus readScaleParams(std::istream &in, point_t &about, double &coef);

  Rectangle::Rectangle(point_t a, point_t b):
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

  Rectangle::Rectangle(rectangle_t r):
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

  double Rectangle::getArea() const
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const
  {
    return {width_, height_, center_};
  }

  void Rectangle::move(point_t to)
  {
    if (!std::isfinite(to.x) || !std::isfinite(to.y)) {
      throw std::invalid_argument("move: bad point");
    }
    center_ = to;
  }

  void Rectangle::move(double dx, double dy)
  {
    if (!std::isfinite(dx) || !std::isfinite(dy)) {
      throw std::invalid_argument("move: bad shift");
    }
    center_.x += dx;
    center_.y += dy;
  }

  void Rectangle::scale(double coef)
  {
    if (!std::isfinite(coef) || coef <= 0.0) {
      throw std::invalid_argument("scale: bad coef");
    }
    width_ *= coef;
    height_ *= coef;
  }

  Square::Square(point_t center, double side):
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

  Square::Square(rectangle_t r):
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

  double Square::getArea() const
  {
    return side_ * side_;
  }

  rectangle_t Square::getFrameRect() const
  {
    return {side_, side_, center_};
  }

  void Square::move(point_t to)
  {
    if (!std::isfinite(to.x) || !std::isfinite(to.y)) {
      throw std::invalid_argument("move: bad point");
    }
    center_ = to;
  }

  void Square::move(double dx, double dy)
  {
    if (!std::isfinite(dx) || !std::isfinite(dy)) {
      throw std::invalid_argument("move: bad shift");
    }
    center_.x += dx;
    center_.y += dy;
  }

  void Square::scale(double coef)
  {
    if (!std::isfinite(coef) || coef <= 0.0) {
      throw std::invalid_argument("scale: bad coef");
    }
    side_ *= coef;
  }

  Xquare::Xquare(point_t center, double diagonal):
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

  Xquare::Xquare(rectangle_t r):
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

  double Xquare::getArea() const
  {
    return (diagonal_ * diagonal_) / 2.0;
  }

  rectangle_t Xquare::getFrameRect() const
  {
    return {diagonal_, diagonal_, center_};
  }

  void Xquare::move(point_t to)
  {
    if (!std::isfinite(to.x) || !std::isfinite(to.y)) {
      throw std::invalid_argument("move: bad point");
    }
    center_ = to;
  }

  void Xquare::move(double dx, double dy)
  {
    if (!std::isfinite(dx) || !std::isfinite(dy)) {
      throw std::invalid_argument("move: bad shift");
    }
    center_.x += dx;
    center_.y += dy;
  }

  void Xquare::scale(double coef)
  {
    if (!std::isfinite(coef) || coef <= 0.0) {
      throw std::invalid_argument("scale: bad coef");
    }
    diagonal_ *= coef;
  }

  void scaleRelative(Shape &shp, point_t about, double coef)
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
    const point_t old_center = shp.getFrameRect().pos;
    const point_t new_center = {
      about.x + (old_center.x - about.x) * coef,
      about.y + (old_center.y - about.y) * coef
    };
    shp.scale(coef);
    shp.move(new_center);
  }

  void scaleAllRelative(Shape *const *shps, size_t size, point_t about, double coef)
  {
    for (size_t i = 0; i < size; ++i) {
      scaleRelative(*shps[i], about, coef);
    }
  }

  rectangle_t getAllShapesFrameRect(Shape *const *shps, size_t size)
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

  void outputParams(std::ostream &out, Shape *const *shps, size_t size)
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
      out << "(" << r.pos.x << ", " << r.pos.y << ") " << r.height << " " << r.width << "\n";
    }

    const rectangle_t all = getAllShapesFrameRect(shps, size);
    out << "(" << all.pos.x << ", " << all.pos.y << ") " << all.height << " " << all.width << "\n";
  }

  void removeArray(Shape **shps, size_t size)
  {
    for (size_t i = 0; i < size; ++i) {
      delete shps[i];
      shps[i] = nullptr;
    }
  }

  ReadStatus readScaleParams(std::istream &in, point_t &about, double &coef)
  {
    if (!(in >> about.x >> about.y >> coef)) {
      return ReadStatus::bad_input;
    }
    if (!std::isfinite(about.x) || !std::isfinite(about.y)) {
      return ReadStatus::bad_input;
    }
    if (!std::isfinite(coef) || coef <= 0.0) {
      return ReadStatus::bad_coef;
    }
    return ReadStatus::ok;
  }
}

int main()
{
  int err = 0;

  const size_t shape_count = 6;
  draw::Shape *shapes[shape_count] = {};

  try {
    shapes[0] = new draw::Rectangle({0.0, 0.0}, {10.0, 5.0});
    shapes[1] = new draw::Rectangle(draw::rectangle_t{4.0, 8.0, {-5.0, 3.0}});
    shapes[2] = new draw::Square({2.0, 2.0}, 6.0);
    shapes[3] = new draw::Square(draw::rectangle_t{5.0, 5.0, {10.0, -2.0}});
    shapes[4] = new draw::Xquare({-3.0, -3.0}, 8.0);
    shapes[5] = new draw::Xquare(draw::rectangle_t{10.0, 10.0, {0.0, 12.0}});
  } catch (const std::exception &e) {
    std::cerr << "init error: " << e.what() << "\n";
    err = 2;
  } catch (...) {
    std::cerr << "init error\n";
    err = 2;
  }

  if (err == 0) {
    draw::outputParams(std::cout, shapes, shape_count);

    draw::point_t about = {0.0, 0.0};
    double coef = 0.0;

    const draw::ReadStatus st = draw::readScaleParams(std::cin, about, coef);
    if (st == draw::ReadStatus::bad_input) {
      std::cerr << "bad input\n";
      err = 1;
    } else if (st == draw::ReadStatus::bad_coef) {
      std::cerr << "bad scale coef\n";
      err = 1;
    } else {
      try {
        draw::scaleAllRelative(shapes, shape_count, about, coef);
        draw::outputParams(std::cout, shapes, shape_count);
      } catch (const std::exception &e) {
        std::cerr << "scale error: " << e.what() << "\n";
        err = 2;
      } catch (...) {
        std::cerr << "scale error\n";
        err = 2;
      }
    }
  }

  draw::removeArray(shapes, shape_count);
  return err;
}
