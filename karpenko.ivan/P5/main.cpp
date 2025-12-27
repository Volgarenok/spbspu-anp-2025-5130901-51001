#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

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
    virtual ~Shape() noexcept = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& point) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void scale(double coefficient) = 0;
  };

  struct Rectangle final: public Shape
  {
  public:
    Rectangle(double width, double height, const point_t& center) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double coefficient) override;

  private:
    double width_;
    double height_;
    point_t center_;
  };

  Rectangle::Rectangle(double width, double height, const point_t& center) noexcept:
    width_(width),
    height_(height),
    center_(center)
  {}

  double Rectangle::getArea() const noexcept
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    rectangle_t frame;
    frame.width = width_;
    frame.height = height_;
    frame.pos = center_;
    return frame;
  }

  void Rectangle::move(const point_t& point) noexcept
  {
    center_ = point;
  }

  void Rectangle::move(double dx, double dy) noexcept
  {
    center_.x += dx;
    center_.y += dy;
  }

  void Rectangle::scale(double coefficient)
  {
    if (coefficient <= 0.0)
    {
      throw std::invalid_argument("Scaling coefficient must be positive");
    }
    width_ *= coefficient;
    height_ *= coefficient;
  }

  struct Triangle final: public Shape
  {
  public:
    Triangle(const point_t& a, const point_t& b, const point_t& c) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double coefficient) override;

  private:
    point_t getCenter() const noexcept;
    point_t getScaledVertex(const point_t& vertex, const point_t& center, double coefficient) const noexcept;
    
    point_t vertexA_;
    point_t vertexB_;
    point_t vertexC_;
  };

  Triangle::Triangle(const point_t& a, const point_t& b, const point_t& c) noexcept:
    vertexA_(a),
    vertexB_(b),
    vertexC_(c)
  {}

  double Triangle::getArea() const noexcept
  {
    double part1 = (vertexB_.x - vertexA_.x) * (vertexC_.y - vertexA_.y);
    double part2 = (vertexC_.x - vertexA_.x) * (vertexB_.y - vertexA_.y);
    return 0.5 * std::fabs(part1 - part2);
  }

  rectangle_t Triangle::getFrameRect() const noexcept
  {
    double minX = std::min({vertexA_.x, vertexB_.x, vertexC_.x});
    double maxX = std::max({vertexA_.x, vertexB_.x, vertexC_.x});
    double minY = std::min({vertexA_.y, vertexB_.y, vertexC_.y});
    double maxY = std::max({vertexA_.y, vertexB_.y, vertexC_.y});

    double width = maxX - minX;
    double height = maxY - minY;
    point_t center = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};

    rectangle_t frame;
    frame.width = width;
    frame.height = height;
    frame.pos = center;
    return frame;
  }

  void Triangle::move(const point_t& point) noexcept
  {
    point_t center = getCenter();
    double dx = point.x - center.x;
    double dy = point.y - center.y;
    
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
  }

  void Triangle::move(double dx, double dy) noexcept
  {
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
  }

  point_t Triangle::getCenter() const noexcept
  {
    return {
      (vertexA_.x + vertexB_.x + vertexC_.x) / 3.0,
      (vertexA_.y + vertexB_.y + vertexC_.y) / 3.0
    };
  }

  point_t Triangle::getScaledVertex(const point_t& vertex, const point_t& center, double coefficient) const noexcept
  {
    return {
      center.x + (vertex.x - center.x) * coefficient,
      center.y + (vertex.y - center.y) * coefficient
    };
  }

  void Triangle::scale(double coefficient)
  {
    if (coefficient <= 0.0)
    {
      throw std::invalid_argument("Scaling coefficient must be positive");
    }
    
    point_t center = getCenter();
    vertexA_ = getScaledVertex(vertexA_, center, coefficient);
    vertexB_ = getScaledVertex(vertexB_, center, coefficient);
    vertexC_ = getScaledVertex(vertexC_, center, coefficient);
  }

  struct ComplexQuad final: public Shape
  {
  public:
    ComplexQuad(const point_t& a, const point_t& b, const point_t& c, const point_t& d) noexcept;
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void scale(double coefficient) override;

  private:
    point_t getCenter() const noexcept;
    double triangleArea(const point_t& a, const point_t& b, const point_t& c) const noexcept;
    
    point_t vertexA_;
    point_t vertexB_;
    point_t vertexC_;
    point_t vertexD_;
  };

  ComplexQuad::ComplexQuad(const point_t& a, const point_t& b, const point_t& c, const point_t& d) noexcept:
    vertexA_(a),
    vertexB_(b),
    vertexC_(c),
    vertexD_(d)
  {}

  double ComplexQuad::getArea() const noexcept
  {
    double area1 = triangleArea(vertexA_, vertexB_, vertexC_);
    double area2 = triangleArea(vertexA_, vertexC_, vertexD_);
    return area1 + area2;
  }

  rectangle_t ComplexQuad::getFrameRect() const noexcept
  {
    double minX = std::min({vertexA_.x, vertexB_.x, vertexC_.x, vertexD_.x});
    double maxX = std::max({vertexA_.x, vertexB_.x, vertexC_.x, vertexD_.x});
    double minY = std::min({vertexA_.y, vertexB_.y, vertexC_.y, vertexD_.y});
    double maxY = std::max({vertexA_.y, vertexB_.y, vertexC_.y, vertexD_.y});

    double width = maxX - minX;
    double height = maxY - minY;
    point_t center = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};

    rectangle_t frame;
    frame.width = width;
    frame.height = height;
    frame.pos = center;
    return frame;
  }

  void ComplexQuad::move(const point_t& point) noexcept
  {
    point_t center = getCenter();
    double dx = point.x - center.x;
    double dy = point.y - center.y;
    
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
    vertexD_.x += dx;
    vertexD_.y += dy;
  }

  void ComplexQuad::move(double dx, double dy) noexcept
  {
    vertexA_.x += dx;
    vertexA_.y += dy;
    vertexB_.x += dx;
    vertexB_.y += dy;
    vertexC_.x += dx;
    vertexC_.y += dy;
    vertexD_.x += dx;
    vertexD_.y += dy;
  }

  point_t ComplexQuad::getCenter() const noexcept
  {
    const double epsilon = 1e-9;
    double denominator = (vertexA_.x - vertexC_.x) * (vertexB_.y - vertexD_.y) -
                       (vertexA_.y - vertexC_.y) * (vertexB_.x - vertexD_.x);

    if (std::fabs(denominator) < epsilon)
    {
      return {
        (vertexA_.x + vertexB_.x + vertexC_.x + vertexD_.x) / 4.0,
        (vertexA_.y + vertexB_.y + vertexC_.y + vertexD_.y) / 4.0
      };
    }
    else
    {
      double t = ((vertexA_.x - vertexB_.x) * (vertexB_.y - vertexD_.y) -
                 (vertexA_.y - vertexB_.y) * (vertexB_.x - vertexD_.x)) / denominator;

      return {
        vertexA_.x + t * (vertexC_.x - vertexA_.x),
        vertexA_.y + t * (vertexC_.y - vertexA_.y)
      };
    }
  }

  double ComplexQuad::triangleArea(const point_t& a, const point_t& b, const point_t& c) const noexcept
  {
    double part1 = (b.x - a.x) * (c.y - a.y);
    double part2 = (c.x - a.x) * (b.y - a.y);
    return 0.5 * std::fabs(part1 - part2);
  }

  void ComplexQuad::scale(double coefficient)
  {
    if (coefficient <= 0.0)
    {
      throw std::invalid_argument("Scaling coefficient must be positive");
    }
    
    point_t center = getCenter();
    
    auto scaleVertex = [&center, coefficient](const point_t& vertex) {
      return point_t{
        center.x + (vertex.x - center.x) * coefficient,
        center.y + (vertex.y - center.y) * coefficient
      };
    };
    
    vertexA_ = scaleVertex(vertexA_);
    vertexB_ = scaleVertex(vertexB_);
    vertexC_ = scaleVertex(vertexC_);
    vertexD_ = scaleVertex(vertexD_);
  }

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

  rectangle_t getOverallFrameRect(const Shape* const* shapes, size_t count)
  {
    const rectangle_t emptyFrame = {0.0, 0.0, {0.0, 0.0}};
    if (count == 0)
    {
      return emptyFrame;
    }

    rectangle_t firstFrame = shapes[0]->getFrameRect();
    double left = firstFrame.pos.x - firstFrame.width / 2.0;
    double right = firstFrame.pos.x + firstFrame.width / 2.0;
    double bottom = firstFrame.pos.y - firstFrame.height / 2.0;
    double top = firstFrame.pos.y + firstFrame.height / 2.0;

    for (size_t i = 1; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      double currentLeft = frame.pos.x - frame.width / 2.0;
      double currentRight = frame.pos.x + frame.width / 2.0;
      double currentBottom = frame.pos.y - frame.height / 2.0;
      double currentTop = frame.pos.y + frame.height / 2.0;

      left = std::min(left, currentLeft);
      right = std::max(right, currentRight);
      bottom = std::min(bottom, currentBottom);
      top = std::max(top, currentTop);
    }

    double width = right - left;
    double height = top - bottom;
    point_t center = {(left + right) / 2.0, (bottom + top) / 2.0};
    
    rectangle_t result;
    result.width = width;
    result.height = height;
    result.pos = center;
    return result;
  }

  void printShapeInfo(const Shape* shape, size_t index)
  {
    std::cout << "Shape " << index + 1 << ":\n";
    std::cout << "  Area: " << std::fixed << std::setprecision(2) << shape->getArea() << "\n";

    rectangle_t frame = shape->getFrameRect();
    std::cout << "  Frame Rect:\n";
    std::cout << "    Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    std::cout << "    Width: " << frame.width << "\n";
    std::cout << "    Height: " << frame.height << "\n";
  }

  double printAllShapesInfo(const Shape* const* shapes, size_t count, const std::string& title)
  {
    std::cout << "\n" << title << ":\n";
    
    double totalArea = 0.0;
    for (size_t i = 0; i < count; ++i)
    {
      printShapeInfo(shapes[i], i);
      totalArea += shapes[i]->getArea();
      
      if (i < count - 1)
      {
        std::cout << "\n";
      }
    }
    
    return totalArea;
  }

  void printOverallFrameRect(const rectangle_t& frame)
  {
    std::cout << "\nOverall Frame Rect:\n";
    std::cout << "  Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    std::cout << "  Width: " << frame.width << "\n";
    std::cout << "  Height: " << frame.height << "\n";
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

  double totalArea = printAllShapesInfo(const_cast< const Shape** >(shapes), SHAPE_COUNT, "Before scaling");
  std::cout << "\nTotal area: " << totalArea << "\n";

  rectangle_t overallFrame = getOverallFrameRect(const_cast< const Shape** >(shapes), SHAPE_COUNT);
  printOverallFrameRect(overallFrame);

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

  try
  {
    scaleShapes(shapes, SHAPE_COUNT, scalePoint, coefficient);
  }
  catch (const std::invalid_argument& e)
  {
    std::cerr << "Error during scaling: " << e.what() << "\n";
    for (size_t i = 0; i < SHAPE_COUNT; ++i)
    {
      delete shapes[i];
    }
    return 1;
  }

  totalArea = printAllShapesInfo(const_cast< const Shape** >(shapes), SHAPE_COUNT, "After scaling");
  std::cout << "\nTotal area: " << totalArea << "\n";

  overallFrame = getOverallFrameRect(const_cast< const Shape** >(shapes), SHAPE_COUNT);
  printOverallFrameRect(overallFrame);

  for (size_t i = 0; i < SHAPE_COUNT; ++i)
  {
    delete shapes[i];
  }

  return 0;
}
