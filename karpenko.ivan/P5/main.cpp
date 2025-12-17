#include <iostream>
#include <cmath>
#include <algorithm>

namespace karpenko
{
  struct point_t
  {
    double x;
    double y;
  };

  struct rectangle_t
  {
    double width;
    double height;
    point_t pos;
  };

  struct Shape
  {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(const point_t& point) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coefficient) = 0;
    virtual ~Shape() = default;
  };

  struct Rectangle : Shape
  {
    double width;
    double height;
    point_t center;

    Rectangle(double width, double height, const point_t& center) :
      width(width),
      height(height),
      center(center)
    {
    }

    double getArea() const override
    {
      return width * height;
    }

    rectangle_t getFrameRect() const override
    {
      rectangle_t result = {width, height, center};
      return result;
    }

    void move(const point_t& point) override
    {
      center = point;
    }

    void move(double dx, double dy) override
    {
      center.x += dx;
      center.y += dy;
    }

    void scale(double coefficient) override
    {
      width *= coefficient;
      height *= coefficient;
    }
  };

  struct Triangle : Shape
  {
    point_t vertexA;
    point_t vertexB;
    point_t vertexC;
    point_t center;

    Triangle(const point_t& a, const point_t& b, const point_t& c) :
      vertexA(a),
      vertexB(b),
      vertexC(c)
    {
      calculateCenter();
    }

  private:
    void calculateCenter()
    {
      center.x = (vertexA.x + vertexB.x + vertexC.x) / 3.0;
      center.y = (vertexA.y + vertexB.y + vertexC.y) / 3.0;
    }

  public:
    double getArea() const override
    {
      return 0.5 * std::fabs(
        (vertexB.x - vertexA.x) * (vertexC.y - vertexA.y) -
        (vertexC.x - vertexA.x) * (vertexB.y - vertexA.y)
      );
    }

    rectangle_t getFrameRect() const override
    {
      double minX = std::min({vertexA.x, vertexB.x, vertexC.x});
      double maxX = std::max({vertexA.x, vertexB.x, vertexC.x});
      double minY = std::min({vertexA.y, vertexB.y, vertexC.y});
      double maxY = std::max({vertexA.y, vertexB.y, vertexC.y});

      double width = maxX - minX;
      double height = maxY - minY;
      point_t center = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};
      
      rectangle_t result = {width, height, center};
      return result;
    }

    void move(const point_t& point) override
    {
      double dx = point.x - center.x;
      double dy = point.y - center.y;
      move(dx, dy);
    }

    void move(double dx, double dy) override
    {
      vertexA.x += dx;
      vertexA.y += dy;
      vertexB.x += dx;
      vertexB.y += dy;
      vertexC.x += dx;
      vertexC.y += dy;
      center.x += dx;
      center.y += dy;
    }

    void scale(double coefficient) override
    {
      vertexA.x = center.x + (vertexA.x - center.x) * coefficient;
      vertexA.y = center.y + (vertexA.y - center.y) * coefficient;
      vertexB.x = center.x + (vertexB.x - center.x) * coefficient;
      vertexB.y = center.y + (vertexB.y - center.y) * coefficient;
      vertexC.x = center.x + (vertexC.x - center.x) * coefficient;
      vertexC.y = center.y + (vertexC.y - center.y) * coefficient;
    }
  };

  struct ComplexQuad : Shape
  {
    point_t vertexA;
    point_t vertexB;
    point_t vertexC;
    point_t vertexD;
    point_t center;

    ComplexQuad(const point_t& a, const point_t& b,
                const point_t& c, const point_t& d) :
      vertexA(a),
      vertexB(b),
      vertexC(c),
      vertexD(d)
    {
      calculateCenter();
    }

  private:
    void calculateCenter()
    {
      center.x = (vertexA.x + vertexB.x + vertexC.x + vertexD.x) / 4.0;
      center.y = (vertexA.y + vertexB.y + vertexC.y + vertexD.y) / 4.0;
    }
  };
}

int main()
{
  return 0;
}
