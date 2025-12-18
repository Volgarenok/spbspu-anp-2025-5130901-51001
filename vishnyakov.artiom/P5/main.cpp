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
    virtual void move(const point_t &newPos) = 0;
    virtual void scale(double coefficient);
    virtual ~Shape() = default;
  };

  struct Concave:Shape {
    point_t spot1, spot2, spot3, spot4, center;
    Concave(point_t a, point_t b, point_t c, point_t d);
    rectangle_t getFrameRect() override;
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;
    void scale(double coefficient) override;
  };

  struct Triangle:Shape{
    point_t spot1, spot2, spot3, center;
    Triangle(point_t a, point_t b, point_t c);
    rectangle_t getFrameRect() override;
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;
    void scale(double coefficient) override;
  };
}

int main()
{

}


vishnyakov::Triangle::Triangle(point_t a, point_t b, point_t c):
 spot1{a}, spot2{b}, spot3{c}, center{point_t {(a.x+b.x+c.x) / 3.0, (a.y+b.y+c.y) / 3.0}}
{}

vishnyakov::rectangle_t vishnyakov::Triangle::getFrameRect()
{
  double minx = std::min(spot1.x, spot2.x, spot3.x);
  double miny = std::min(spot1.y, spot2.y, spot3.y);
  double maxx = std::max(spot1.x, spot2.x, spot3.x);
  double maxy = std::max(spot1.y, spot2.y, spot3.y);

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
  return sqrt(p* (p - side1) * (p - side2) * (p - side3));
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

void vishnyakov::Triangle::move(const point_t &newPos)
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