#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

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
      double denominator = (vertexA.x - vertexC.x) * (vertexB.y - vertexD.y) -
                         (vertexA.y - vertexC.y) * (vertexB.x - vertexD.x);

      if (std::fabs(denominator) < 1e-9)
      {
        center.x = (vertexA.x + vertexB.x + vertexC.x + vertexD.x) / 4.0;
        center.y = (vertexA.y + vertexB.y + vertexC.y + vertexD.y) / 4.0;
      }
      else
      {
        double t = ((vertexA.x - vertexB.x) * (vertexB.y - vertexD.y) -
                   (vertexA.y - vertexB.y) * (vertexB.x - vertexD.x)) / denominator;

        center.x = vertexA.x + t * (vertexC.x - vertexA.x);
        center.y = vertexA.y + t * (vertexC.y - vertexA.y);
      }
    }

    double triangleArea(const point_t& a, const point_t& b, const point_t& c) const
    {
      return 0.5 * std::fabs(
        (b.x - a.x) * (c.y - a.y) -
        (c.x - a.x) * (b.y - a.y)
      );
    }

  public:
    double getArea() const override
    {
      double area1 = triangleArea(vertexA, vertexB, vertexC);
      double area2 = triangleArea(vertexA, vertexC, vertexD);
      return area1 + area2;
    }

    rectangle_t getFrameRect() const override
    {
      double minX = std::min({vertexA.x, vertexB.x, vertexC.x, vertexD.x});
      double maxX = std::max({vertexA.x, vertexB.x, vertexC.x, vertexD.x});
      double minY = std::min({vertexA.y, vertexB.y, vertexC.y, vertexD.y});
      double maxY = std::max({vertexA.y, vertexB.y, vertexC.y, vertexD.y});

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
      vertexD.x += dx;
      vertexD.y += dy;
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
      vertexD.x = center.x + (vertexD.x - center.x) * coefficient;
      vertexD.y = center.y + (vertexD.y - center.y) * coefficient;
    }
  };

  void scaleShapes(Shape** shapes, size_t count, const point_t& point, double coefficient)
  {
    for (size_t i = 0; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      double dx = frame.pos.x - point.x;
      double dy = frame.pos.y - point.y;

      shapes[i]->move(point);
      shapes[i]->scale(coefficient);
      shapes[i]->move(-dx * coefficient, -dy * coefficient);
    }
  }

  rectangle_t getOverallFrameRect(Shape** shapes, size_t count)
  {
    if (count == 0)
    {
      rectangle_t result = {0.0, 0.0, {0.0, 0.0}};
      return result;
    }

    rectangle_t firstFrame = shapes[0]->getFrameRect();
    double minX = firstFrame.pos.x - firstFrame.width / 2.0;
    double maxX = firstFrame.pos.x + firstFrame.width / 2.0;
    double minY = firstFrame.pos.y - firstFrame.height / 2.0;
    double maxY = firstFrame.pos.y + firstFrame.height / 2.0;

    for (size_t i = 1; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      double left = frame.pos.x - frame.width / 2.0;
      double right = frame.pos.x + frame.width / 2.0;
      double bottom = frame.pos.y - frame.height / 2.0;
      double top = frame.pos.y + frame.height / 2.0;

      if (left < minX)
      {
        minX = left;
      }
      if (right > maxX)
      {
        maxX = right;
      }
      if (bottom < minY)
      {
        minY = bottom;
      }
      if (top > maxY)
      {
        maxY = top;
      }
    }

    double width = maxX - minX;
    double height = maxY - minY;
    point_t center = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};
    rectangle_t result = {width, height, center};
    return result;
  }
}

int main()
{
  using namespace karpenko;
  
  const size_t SHAPE_COUNT = 4;
  Shape* shapes[SHAPE_COUNT] = {nullptr};

  try
  {
    shapes[0] = new Rectangle(4.0, 3.0, {0.0, 0.0});
    shapes[1] = new Triangle({-2.0, -1.0}, {2.0, -1.0}, {0.0, 3.0});
    shapes[2] = new ComplexQuad({-1.0, 0.0}, {0.0, -2.0}, {1.0, 0.0}, {0.0, 2.0});
    shapes[3] = new Rectangle(2.0, 5.0, {3.0, 2.0});
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation failed\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  std::cout << "Before scaling:\n";
  double totalArea = 0.0;
  for (size_t i = 0; i < SHAPE_COUNT; ++i)
  {
    std::cout << "Shape " << i + 1 << ":\n";
    std::cout << "  Area: " << std::fixed << std::setprecision(2) << shapes[i]->getArea() << "\n";

    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << "  Frame Rect: center(" << frame.pos.x << ", " << frame.pos.y << "), width " << frame.width << ", height " << frame.height << "\n";

    totalArea += shapes[i]->getArea();
  }

  std::cout << "Total area: " << totalArea << "\n";

  rectangle_t overallFrame = getOverallFrameRect(shapes, SHAPE_COUNT);
  std::cout << "Overall Frame Rect: center(" << overallFrame.pos.x << ", " << overallFrame.pos.y << "), width " << overallFrame.width << ", height " << overallFrame.height << "\n";

  point_t scalePoint;
  double coefficient;

  std::cout << "\nEnter scaling point (x y): ";
  if (!(std::cin >> scalePoint.x >> scalePoint.y))
  {
    std::cerr << "Error: invalid point coordinates\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  std::cout << "Enter scaling coefficient: ";
  if (!(std::cin >> coefficient))
  {
    std::cerr << "Error: invalid coefficient\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  if (coefficient <= 0.0)
  {
    std::cerr << "Error: scaling coefficient must be positive\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  scaleShapes(shapes, SHAPE_COUNT, scalePoint, coefficient);

  std::cout << "\nAfter scaling:\n";
  totalArea = 0.0;
  for (size_t i = 0; i < SHAPE_COUNT; ++i)
  {
    std::cout << "Shape " << i + 1 << ":\n";
    std::cout << "  Area: " << std::fixed << std::setprecision(2) << shapes[i]->getArea() << "\n";

    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << "  Frame Rect: center(" << frame.pos.x << ", " << frame.pos.y << "), width " << frame.width << ", height " << frame.height << "\n";

    totalArea += shapes[i]->getArea();
  }

  std::cout << "Total area: " << totalArea << "\n";

  overallFrame = getOverallFrameRect(shapes, SHAPE_COUNT);
  std::cout << "Overall Frame Rect: center(" << overallFrame.pos.x << ", " << overallFrame.pos.y << "), width " << overallFrame.width << ", height " << overallFrame.height << "\n";

  for (size_t i = 0; i < SHAPE_COUNT; ++i)
  {
    delete shapes[i];
  }

  return 0;
}
