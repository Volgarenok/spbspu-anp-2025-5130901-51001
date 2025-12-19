#include <iostream>
#include <cmath>

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
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void move(const point_t newPos) = 0;
    virtual void scale(double coefficient) = 0;
    virtual ~Shape() = default;
  };

  struct Rectangle:Shape {
    double width, height;
    point_t center;
    Rectangle(double w, double h, point_t c);
    rectangle_t getFrameRect() override;
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t newPos) override;
    void scale(double coefficient) override;
  };

  struct Concave:Shape {
    point_t spot1, spot2, spot3, spot4, center;
    Concave(point_t a, point_t b, point_t c, point_t d);
    rectangle_t getFrameRect() override;
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t newPos) override;
    void scale(double coefficient) override;
  };

  struct Triangle:Shape{
    point_t spot1, spot2, spot3, center;
    Triangle(point_t a, point_t b, point_t c);
    rectangle_t getFrameRect() override;
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t newPos) override;
    void scale(double coefficient) override;
  };

  double min(double * nums, size_t size);
  double max(double * nums, size_t size);
  rectangle_t FrameRect_for_all_shapes(Shape ** shapes, size_t size);
  void Scale_Shapes_relative_to_the_point(Shape ** shapes, size_t size, point_t point, double coefficient);
}

int main()
{

}

double vishnyakov::min(double * nums, size_t size)
{
  double min_value = nums[0];
  for (size_t i = 1; i < size; ++i)
  {
    min_value = nums[i-1] > nums[i] ? nums[i] : nums[i-1];
  }
  return min_value;
}

double vishnyakov::max(double * nums, size_t size)
{
  double max_value = nums[0];
  for (size_t i = 1; i < size; ++i)
  {
    max_value = nums[i-1] < nums[i] ? nums[i] : nums[i-1];
  }
  return max_value;
}

vishnyakov::rectangle_t vishnyakov::FrameRect_for_all_shapes(Shape ** shapes, size_t size)
{
  if (size == 0)
  {
    return rectangle_t {0.0, 0.0, {0.0, 0.0}};
  }

  rectangle_t frame = shapes[0]->getFrameRect();

  double minx = frame.pos.x - frame.width / 2.0;
  double maxx = frame.pos.x + frame.width / 2.0;
  double miny = frame.pos.x - frame.height / 2.0;
  double maxy = frame.pos.x + frame.height / 2.0;

  for (size_t i = 1; i < size; ++ i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    minx = frame.pos.x - frame.width / 2.0 < minx ? frame.pos.x - frame.width / 2.0 : minx;
    maxx = frame.pos.x + frame.width / 2.0 > maxx ? frame.pos.x + frame.width / 2.0 : maxx;
    miny = frame.pos.y - frame.width / 2.0 < miny ? frame.pos.y - frame.width / 2.0 : miny;
    maxy = frame.pos.y + frame.width / 2.0 > maxy ? frame.pos.y + frame.width / 2.0 : maxy;
  }

  double width = maxx - minx;
  double height = maxy - miny;

  point_t center = {(minx + maxx) / 2.0, (miny + maxy) / 2.0};

  return rectangle_t {width, height, center};
}

void vishnyakov::Scale_Shapes_relative_to_the_point(Shape ** shapes, size_t size, point_t point, double coefficient)
{
  for (size_t i = 0; i < size; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();

    double dx = point.x - frame.pos.x;
    double dy = point.y - frame.pos.y;

    shapes[i]->move(point);
    shapes[i]->scale(coefficient);
    shapes[i]->move(-dx * coefficient, -dy * coefficient);
  }
}

// Четырёхугольник

vishnyakov::Rectangle::Rectangle(double w, double h, point_t c):
 width{w},
 height{h},
 center{c}
{}

vishnyakov::rectangle_t vishnyakov::Rectangle::getFrameRect()
{
  return rectangle_t {width, height, center};
}

double vishnyakov::Rectangle::getArea() const
{
  return width * height;
}

void vishnyakov::Rectangle::move(double dx, double dy)
{
  center.x += dx;
  center.y += dy;
}

void vishnyakov::Rectangle::move(const point_t newPos)
{
  center = newPos;
}

void vishnyakov::Rectangle::scale(double coefficient)
{
  width *= coefficient;
  height *= coefficient;
}

// Невыпуклый четырёхугольник

vishnyakov::Concave::Concave(point_t a, point_t b, point_t c, point_t d):
 spot1{a},
 spot2{b},
 spot3{c},
 spot4{d},
 center{point_t {(a.x + b.x + c.x + d.x) / 4.0, (a.y + b.y + c.y + d.y) / 4.0}}
{}

vishnyakov::rectangle_t vishnyakov::Concave::getFrameRect()
{
  double x_vlaues[4] = {spot1.x, spot2.x, spot3.x, spot4.x};
  double y_vlaues[4] = {spot1.y, spot2.y, spot3.y, spot4.y};

  double minx = vishnyakov::min(x_vlaues, 4);
  double maxx = vishnyakov::max(x_vlaues, 4);
  double miny = vishnyakov::min(y_vlaues, 4);
  double maxy = vishnyakov::max(y_vlaues, 4);

  double width = maxx - minx;
  double height = maxy - miny;

  point_t center = {(maxx - minx) / 2.0, (maxy - miny) / 2.0};

  return rectangle_t {width, height, center};
}

double vishnyakov::Concave::getArea() const
{
  double tmp_value_1 = (spot1.x - spot2.x) * (spot1.y + spot2.y);
  double tmp_value_2 = (spot2.x - spot3.x) * (spot2.y + spot3.y);
  double tmp_value_3 = (spot3.x - spot4.x) * (spot3.y + spot4.y);
  double tmp_value_4 = (spot4.x - spot1.x) * (spot4.y + spot1.y);

  return std::abs(tmp_value_1 + tmp_value_2 + tmp_value_3 + tmp_value_4);
}

void vishnyakov::Concave::move(double dx, double dy)
{
  spot1.x += dx;
  spot2.x += dx;
  spot3.x += dx;
  spot4.x += dx;

  spot1.y += dy;
  spot2.y += dy;
  spot3.y += dy;
  spot4.y += dy;

  center.x += dx;
  center.y += dy;
}

void vishnyakov::Concave::move(const point_t newPos)
{
  double dx = newPos.x - center.x;
  double dy = newPos.y - center.y;

  vishnyakov::Concave::move(dx, dy);
}

void vishnyakov::Concave::scale(double coefficient)
{
  spot1.x = center.x + (spot1.x - center.x) * coefficient;
  spot2.x = center.x + (spot2.x - center.x) * coefficient;
  spot3.x = center.x + (spot3.x - center.x) * coefficient;
  spot4.x = center.x + (spot4.x - center.x) * coefficient;

  spot1.y = center.y + (spot1.y - center.y) * coefficient;
  spot2.y = center.y + (spot2.y - center.y) * coefficient;
  spot3.y = center.y + (spot3.y - center.y) * coefficient;
  spot4.y = center.y + (spot4.y - center.y) * coefficient;
}

// Треугольник

vishnyakov::Triangle::Triangle(point_t a, point_t b, point_t c):
 spot1{a},
 spot2{b},
 spot3{c},
 center{point_t {(a.x + b.x + c.x) / 3.0, (a.y + b.y + c.y) / 3.0}}
{}

vishnyakov::rectangle_t vishnyakov::Triangle::getFrameRect()
{
  double x_values[3] = {spot1.x, spot2.x, spot3.x};
  double y_values[3] = {spot1.y, spot2.y, spot3.y};

  double minx = vishnyakov::min(x_values, 3);
  double miny = vishnyakov::max(x_values, 3);
  double maxx = vishnyakov::min(y_values, 3);
  double maxy = vishnyakov::max(y_values, 3);

  double width = maxx - minx;
  double height = maxy - miny;

  point_t center = {(minx + maxx) / 2.0, (miny + maxy) / 2.0};

  return rectangle_t{width, height, center};
}

double vishnyakov::Triangle::getArea() const
{
  double side1 = 0, side2 = 0 , side3 = 0, p = 0;

  side1 = sqrt((spot1.x - spot2.x) * (spot1.x - spot2.x) + (spot1.y - spot2.y) * (spot1.y - spot2.y));
  side2 = sqrt((spot2.x - spot3.x) * (spot2.x - spot3.x) + (spot2.y - spot3.y) * (spot2.y - spot3.y));
  side3 = sqrt((spot1.x - spot3.x) * (spot1.x - spot3.x) + (spot1.y - spot3.y) * (spot1.y - spot3.y));

  p = 0.5 * (side1 + side2 + side3);

  return sqrt(p * (p - side1) * (p - side2) * (p - side3));
}

void vishnyakov::Triangle::move(double dx, double dy)
{
  spot1.x += dx;
  spot2.x += dx;
  spot3.x += dx;

  spot1.y += dy;
  spot2.y += dy;
  spot3.y += dy;

  center.x += dx;
  center.y += dy;
}

void vishnyakov::Triangle::move(const point_t newPos)
{
  double dx = newPos.x - center.x;
  double dy = newPos.y - center.y;

  vishnyakov::Triangle::move(dx, dy);
}

void vishnyakov::Triangle::scale(double coefficient)
{
  spot1.x = center.x + (spot1.x - center.x) * coefficient;
  spot2.x = center.x + (spot2.x - center.x) * coefficient;
  spot3.x = center.x + (spot3.x - center.x) * coefficient;

  spot1.y = center.y + (spot1.y - center.y) * coefficient;
  spot2.y = center.y + (spot2.y - center.y) * coefficient;
  spot3.y = center.y + (spot3.y - center.y) * coefficient;
}