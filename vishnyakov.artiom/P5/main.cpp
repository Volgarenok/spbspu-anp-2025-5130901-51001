#include <iostream>
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
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;
    void scale(double coefficient) override;
  };

  struct Triangle:Shape{
    point_t spot1, spot2, spot3, center;
    Triangle(point_t a, point_t b, point_t c);
    double getArea() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;
    void scale(double coefficient) override;
  };
}

int main()
{
  
}
