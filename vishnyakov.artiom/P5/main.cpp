#include <iostream>
#include <cmath>
#include <algorithm>

namespace vishnyakov
{
  struct point_t {
    double x, y;
  };

  struct rectangle_t {
    double width, height;
    point_t pos;
  };

  struct Shape {
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void move(const point_t newPos) noexcept = 0;
    virtual void scale(double coefficient) noexcept = 0;
    void checkedScale(double coefficient);
  };

  struct Rectangle: Shape {
    Rectangle(double w, double h, point_t c);
    rectangle_t getFrameRect() const noexcept override;
    double getArea() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t newPos) noexcept override;
    void scale(double coefficient) noexcept override;
  private:
    double width_, height_;
    point_t center_;
  };

  struct Concave: Shape {
    Concave(point_t a, point_t b, point_t c, point_t d);
    rectangle_t getFrameRect() const noexcept override;
    double getArea() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t newPos) noexcept override;
    void scale(double coefficient) noexcept override;
  private:
    point_t spot1_, spot2_, spot3_, spot4_, center_;
  };

  struct Triangle: Shape {
    Triangle(point_t a, point_t b, point_t c);
    rectangle_t getFrameRect() const noexcept override;
    double getArea() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t newPos) noexcept override;
    void scale(double coefficient) noexcept override;
  private:
    point_t spot1_, spot2_, spot3_, center_;
  };

  void movePoint(point_t &pos, double dx, double dy);
  rectangle_t getFullFrameRect(const Shape* const *shapes, size_t size);
  void scaleAllShapes(Shape **shapes, size_t size, point_t point, double coefficient);
  void outputInformation(const Shape* const *shapes, size_t size);
}

int main()
{
  using namespace vishnyakov;

  size_t amount_of_shapes = 3;
  Shape * Shapes[amount_of_shapes] = {};

  try
  {
    Shapes[0] = new Rectangle(2.0, 2.0, {0.0, 0.0});
    Shapes[1] = new Triangle({-4.0, -2.0}, {-2.0, 6.0}, {2.0, 0.0});
    Shapes[2] = new Concave({4.0, 3.0}, {-4.0, -2.0}, {5.0, -3.0}, {-1.0, -1.0});

    outputInformation(Shapes, amount_of_shapes);

    double coefficient = 0;
    point_t point = {0.0, 0.0};

    std::cout << "\nEnter point about which shapes will be scale: ";
    if (!(std::cin >> point.x >> point.y))
    {
      std::cerr << "Error: invalid point coordinates\n";

      for (size_t i = 0; i < amount_of_shapes; ++i)
      {
        delete Shapes[i];
      }

      return 1;
    }

    std::cout << "Enter coefficient of scalling: ";
    if (!(std::cin >> coefficient) || coefficient <= 0.0)
    {
      std::cerr << "Error: invalid coefficient\n";

      for (size_t i = 0; i < amount_of_shapes; ++i)
      {
        delete Shapes[i];
      }

      return 1;
    }

    scaleAllShapes(Shapes, amount_of_shapes, point, coefficient);

    std::cout << "\nAfter scalling:\n";

    outputInformation(Shapes, amount_of_shapes);
  }
  catch(const std::bad_alloc&)
  {
    std::cerr << "Memory allocation failed\n";
    return 2;
  }

  for (size_t i = 0; i < amount_of_shapes; ++i)
  {
    delete Shapes[i];
  }

  return 0;
}

void vishnyakov::movePoint(point_t &pos, double dx, double dy)
{
  pos.x += dx;
  pos.y += dy;
}

vishnyakov::rectangle_t vishnyakov::getFullFrameRect(const Shape* const *shapes, size_t size)
{
  if (size == 0)
  {
    return rectangle_t{0.0, 0.0, {0.0, 0.0}};
  }

  rectangle_t frame = shapes[0]->getFrameRect();

  double minx = frame.pos.x - frame.width / 2.0;
  double maxx = frame.pos.x + frame.width / 2.0;
  double miny = frame.pos.y - frame.height / 2.0;
  double maxy = frame.pos.y + frame.height / 2.0;

  for (size_t i = 1; i < size; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    minx = frame.pos.x - frame.width / 2.0 < minx ? frame.pos.x - frame.width / 2.0 : minx;
    maxx = frame.pos.x + frame.width / 2.0 > maxx ? frame.pos.x + frame.width / 2.0 : maxx;
    miny = frame.pos.y - frame.height / 2.0 < miny ? frame.pos.y - frame.height / 2.0 : miny;
    maxy = frame.pos.y + frame.height / 2.0 > maxy ? frame.pos.y + frame.height / 2.0 : maxy;
  }

  double width = maxx - minx;
  double height = maxy - miny;

  point_t center = {(minx + maxx) / 2.0, (miny + maxy) / 2.0};

  return rectangle_t {width, height, center};
}

void vishnyakov::scaleAllShapes(Shape **shapes, size_t size, point_t point, double coefficient)
{
  for (size_t i = 0; i < size; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    double dx = (frame.pos.x - point.x) * (coefficient - 1);
    double dy = (frame.pos.y - point.y) * (coefficient - 1);

    shapes[i]->scale(coefficient);
    shapes[i]->move(dx, dy);
  }
}

void vishnyakov::outputInformation(const Shape* const* shapes, size_t size)
{
  double total_Area = 0.0;

  for (size_t i = 0; i < size; ++i)
  {
    std::cout << "Shape #" << (i+1) << ":\n";
    std::cout << "  Area = " << shapes[i]->getArea() << '\n';
    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << "  FrameRect: center = {" << frame.pos.x << ", " << frame.pos.y;
    std::cout << "}; width = " << frame.width << "; height = " << frame.height << '\n';

    total_Area += shapes[i]->getArea();
  }

  std::cout << "\nTotal Area = " << total_Area << '\n';

  rectangle_t total_FrameRect = getFullFrameRect(shapes, size);
  std::cout << "\nTotal FrameRect: center = {" << total_FrameRect.pos.x << ", " << total_FrameRect.pos.y;
  std::cout << "}; width = " << total_FrameRect.width << "; height = " << total_FrameRect.height << '\n';
}

void vishnyakov::Shape::checkedScale(double coefficient)
{
  if (coefficient <= 0.0)
  {
    throw std::invalid_argument("Error: invalid coefficient\n");
  }
  scale(coefficient);
}

vishnyakov::Rectangle::Rectangle(double w, double h, point_t c):
  width_{w},
  height_{h},
  center_{c}
{}

vishnyakov::rectangle_t vishnyakov::Rectangle::getFrameRect() const noexcept
{
  return rectangle_t{width_, height_, center_};
}

double vishnyakov::Rectangle::getArea() const noexcept
{
  return width_ * height_;
}

void vishnyakov::Rectangle::move(double dx, double dy) noexcept
{
  movePoint(center_, dx, dy);
}

void vishnyakov::Rectangle::move(const point_t newPos) noexcept
{
  center_ = newPos;
}

void vishnyakov::Rectangle::scale(double coefficient) noexcept
{
  width_ *= coefficient;
  height_ *= coefficient;
}

vishnyakov::Concave::Concave(point_t a, point_t b, point_t c, point_t d):
  spot1_{a},
  spot2_{b},
  spot3_{c},
  spot4_{d},
  center_{point_t{(a.x + b.x + c.x + d.x) / 4.0, (a.y + b.y + c.y + d.y) / 4.0}}
{}

vishnyakov::rectangle_t vishnyakov::Concave::getFrameRect() const noexcept
{
  double minx = std::min({spot1_.x, spot2_.x, spot3_.x, spot4_.x});
  double maxx = std::max({spot1_.x, spot2_.x, spot3_.x, spot4_.x});
  double miny = std::min({spot1_.y, spot2_.y, spot3_.y, spot4_.y});
  double maxy = std::max({spot1_.y, spot2_.y, spot3_.y, spot4_.y});

  double width_ = maxx - minx;
  double height = maxy - miny;

  point_t center = {(maxx - minx) / 2.0, (maxy - miny) / 2.0};

  return rectangle_t{width_, height, center};
}

double vishnyakov::Concave::getArea() const noexcept
{
  double tmp_value_1 = (spot1_.x - spot2_.x) * (spot1_.y + spot2_.y);
  double tmp_value_2 = (spot2_.x - spot3_.x) * (spot2_.y + spot3_.y);
  double tmp_value_3 = (spot3_.x - spot4_.x) * (spot3_.y + spot4_.y);
  double tmp_value_4 = (spot4_.x - spot1_.x) * (spot4_.y + spot1_.y);

  double result = std::abs(tmp_value_1 + tmp_value_2 + tmp_value_3 + tmp_value_4);

  return result;
}

void vishnyakov::Concave::move(double dx, double dy) noexcept
{
  movePoint(spot1_, dx, dy);
  movePoint(spot2_, dx, dy);
  movePoint(spot3_, dx, dy);
  movePoint(spot4_, dx, dy);

  movePoint(center_, dx, dy);
}

void vishnyakov::Concave::move(const point_t newPos) noexcept
{
  double dx = newPos.x - center_.x;
  double dy = newPos.y - center_.y;

  move(dx, dy);
}

void vishnyakov::Concave::scale(double coefficient) noexcept
{
  spot1_.x = center_.x + (spot1_.x - center_.x) * coefficient;
  spot2_.x = center_.x + (spot2_.x - center_.x) * coefficient;
  spot3_.x = center_.x + (spot3_.x - center_.x) * coefficient;
  spot4_.x = center_.x + (spot4_.x - center_.x) * coefficient;

  spot1_.y = center_.y + (spot1_.y - center_.y) * coefficient;
  spot2_.y = center_.y + (spot2_.y - center_.y) * coefficient;
  spot3_.y = center_.y + (spot3_.y - center_.y) * coefficient;
  spot4_.y = center_.y + (spot4_.y - center_.y) * coefficient;
}

vishnyakov::Triangle::Triangle(point_t a, point_t b, point_t c):
  spot1_{a},
  spot2_{b},
  spot3_{c},
  center_{point_t {(a.x + b.x + c.x) / 3.0, (a.y + b.y + c.y) / 3.0}}
{}

vishnyakov::rectangle_t vishnyakov::Triangle::getFrameRect() const noexcept
{
  double minx = std::min({spot1_.x, spot2_.x, spot3_.x});
  double maxx = std::max({spot1_.x, spot2_.x, spot3_.x});
  double miny = std::min({spot1_.y, spot2_.y, spot3_.y});
  double maxy = std::max({spot1_.y, spot2_.y, spot3_.y});

  double width_ = maxx - minx;
  double height = maxy - miny;

  point_t center = {(minx + maxx) / 2.0, (miny + maxy) / 2.0};

  return rectangle_t{width_, height, center};
}

double vishnyakov::Triangle::getArea() const noexcept
{
  double side1 = 0, side2 = 0 , side3 = 0, p = 0, result = 0;

  side1 = std::sqrt((spot1_.x - spot2_.x) * (spot1_.x - spot2_.x) + (spot1_.y - spot2_.y) * (spot1_.y - spot2_.y));
  side2 = std::sqrt((spot2_.x - spot3_.x) * (spot2_.x - spot3_.x) + (spot2_.y - spot3_.y) * (spot2_.y - spot3_.y));
  side3 = std::sqrt((spot1_.x - spot3_.x) * (spot1_.x - spot3_.x) + (spot1_.y - spot3_.y) * (spot1_.y - spot3_.y));

  p = 0.5 * (side1 + side2 + side3);

  result = std::sqrt(p * (p - side1) * (p - side2) * (p - side3));

  return result;
}

void vishnyakov::Triangle::move(double dx, double dy) noexcept
{
  movePoint(spot1_, dx, dy);
  movePoint(spot2_, dx, dy);
  movePoint(spot3_, dx, dy);

  movePoint(center_, dx, dy);
}

void vishnyakov::Triangle::move(const point_t newPos) noexcept
{
  double dx = newPos.x - center_.x;
  double dy = newPos.y - center_.y;

  move(dx, dy);
}

void vishnyakov::Triangle::scale(double coefficient) noexcept
{
  spot1_.x = center_.x + (spot1_.x - center_.x) * coefficient;
  spot2_.x = center_.x + (spot2_.x - center_.x) * coefficient;
  spot3_.x = center_.x + (spot3_.x - center_.x) * coefficient;

  spot1_.y = center_.y + (spot1_.y - center_.y) * coefficient;
  spot2_.y = center_.y + (spot2_.y - center_.y) * coefficient;
  spot3_.y = center_.y + (spot3_.y - center_.y) * coefficient;
}

