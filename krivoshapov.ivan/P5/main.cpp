#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace krivoshapov
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

  class Shape
  {
  public:
    virtual ~Shape() = default;

    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    virtual void move(const point_t& newPos) noexcept = 0;

    void scale(double factor);
    virtual void scaleUnchecked(double factor) noexcept = 0;
  };

  class Circle final: public Shape
  {
  public:
    Circle(const point_t& center, double radius);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t& newPos) noexcept override;
    void scaleUnchecked(double factor) noexcept override;

  private:
    point_t center_;
    double radius_;
  };

  class Rectangle final: public Shape
  {
  public:
    Rectangle(const point_t& center, double width, double height);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t& newPos) noexcept override;
    void scaleUnchecked(double factor) noexcept override;

  private:
    point_t center_;
    double width_;
    double height_;
  };

  class Rubber final: public Shape
  {
  public:
    Rubber(const point_t& circleCenter, double radius, const point_t& shapeCenter);

    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(double dx, double dy) noexcept override;
    void move(const point_t& newPos) noexcept override;
    void scaleUnchecked(double factor) noexcept override;

  private:
    point_t circleCenter_;
    double radius_;
    point_t shapeCenter_;
  };

  void Shape::scale(double factor)
  {
    if (factor <= 0.0)
    {
      throw std::invalid_argument("Scale factor must be positive");
    }
    scaleUnchecked(factor);
  }

  Circle::Circle(const point_t& center, double radius):
    center_(center),
    radius_(radius)
  {
    if (radius <= 0.0)
    {
      throw std::invalid_argument("Circle radius must be positive");
    }
  }

  double Circle::getArea() const noexcept
  {
    return M_PI * radius_ * radius_;
  }

  rectangle_t Circle::getFrameRect() const noexcept
  {
    return {2.0 * radius_, 2.0 * radius_, center_};
  }

  void Circle::move(double dx, double dy) noexcept
  {
    center_.x += dx;
    center_.y += dy;
  }

  void Circle::move(const point_t& newPos) noexcept
  {
    center_ = newPos;
  }

  void Circle::scaleUnchecked(double factor) noexcept
  {
    radius_ *= factor;
  }

  Rectangle::Rectangle(const point_t& center, double width, double height) :
    center_(center),
    width_(width),
    height_(height)
  {
    if (width <= 0.0 || height <= 0.0)
    {
      throw std::invalid_argument("Rectangle dimensions must be positive");
    }
  }

  double Rectangle::getArea() const noexcept
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    return {width_, height_, center_};
  }

  void Rectangle::move(double dx, double dy) noexcept
  {
    center_.x += dx;
    center_.y += dy;
  }

  void Rectangle::move(const point_t& newPos) noexcept
  {
    center_ = newPos;
  }

  void Rectangle::scaleUnchecked(double factor) noexcept
  {
    width_ *= factor;
    height_ *= factor;
  }

  Rubber::Rubber(const point_t& circleCenter, double radius, const point_t& shapeCenter) :
    circleCenter_(circleCenter),
    radius_(radius),
    shapeCenter_(shapeCenter)
  {
    if (radius <= 0.0)
    {
      throw std::invalid_argument("Rubber radius must be positive");
    }

    double dx = shapeCenter.x - circleCenter.x;
    double dy = shapeCenter.y - circleCenter.y;
    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist >= radius)
    {
      throw std::invalid_argument("Shape center must be inside the circle");
    }
  }

  double Rubber::getArea() const noexcept
  {
    return M_PI * radius_ * radius_;
  }

  rectangle_t Rubber::getFrameRect() const noexcept
  {
    return {2.0 * radius_, 2.0 * radius_, circleCenter_};
  }

  void Rubber::move(double dx, double dy) noexcept
  {
    circleCenter_.x += dx;
    circleCenter_.y += dy;
    shapeCenter_.x += dx;
    shapeCenter_.y += dy;
  }

  void Rubber::move(const point_t& newPos) noexcept
  {
    double dx = newPos.x - shapeCenter_.x;
    double dy = newPos.y - shapeCenter_.y;
    move(dx, dy);
  }

  void Rubber::scaleUnchecked(double factor) noexcept
  {
    radius_ *= factor;
    double dx = circleCenter_.x - shapeCenter_.x;
    double dy = circleCenter_.y - shapeCenter_.y;
    circleCenter_.x = shapeCenter_.x + dx * factor;
    circleCenter_.y = shapeCenter_.y + dy * factor;
  }

  void scaleShapes(Shape** shapes, size_t count, const point_t& scaleCenter, double factor)
  {
    if (factor <= 0.0)
    {
      throw std::invalid_argument("Scale factor must be positive");
    }

    for (size_t i = 0; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      point_t shapeCenter = frame.pos;

      double dx = shapeCenter.x - scaleCenter.x;
      double dy = shapeCenter.y - scaleCenter.y;

      shapes[i]->move(scaleCenter);
      shapes[i]->scaleUnchecked(factor);

      point_t newCenter;
      newCenter.x = scaleCenter.x + dx * factor;
      newCenter.y = scaleCenter.y + dy * factor;
      shapes[i]->move(newCenter);
    }
  }

  void printShapeInfo(const Shape& shape, size_t index)
  {
    std::cout << "Shape" << index + 1 << ":\n";
    std::cout << "Area: " << shape.getArea() << "\n";

    rectangle_t frame = shape.getFrameRect();
    std::cout << "Frame rectangle center(" << frame.pos.x << ", " << frame.pos.y << "), ";
    std::cout << "width: " << frame.width << ", height: " << frame.height << "\n";
  }

  rectangle_t getOverallFrameRect(const Shape* const* shapes, size_t count)
  {
    if (count == 0)
    {
      return {0.0, 0.0, {0.0, 0.0}};
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

      minX = std::min(minX, left);
      maxX = std::max(maxX, right);
      minY = std::min(minY, bottom);
      maxY = std::max(maxY, top);
    }

    rectangle_t result;
    result.width = maxX - minX;
    result.height = maxY - minY;
    result.pos.x = minX + result.width / 2.0;
    result.pos.y = minY + result.height / 2.0;

    return result;
  }

  double getTotalArea(const Shape* const* shapes, size_t count)
  {
    double total = 0.0;
    for (size_t i = 0; i < count; ++i)
    {
      total += shapes[i]->getArea();
    }
    return total;
  }

  void printAllShapesInfo(const Shape* const* shapes, size_t count)
  {
    for (size_t i = 0; i < count; ++i)
    {
      printShapeInfo(*shapes[i], i);
    }

    double totalArea = getTotalArea(shapes, count);
    std::cout << "\nTotal area: " << totalArea << "\n";

    rectangle_t overallFrame = getOverallFrameRect(shapes, count);
    std::cout << "\nOverall frame rectangle: ";
    std::cout << "center(" << overallFrame.pos.x << ", " << overallFrame.pos.y << "), ";
    std::cout << "width: " << overallFrame.width << ", height: " << overallFrame.height << "\n";
  }

}

int main()
{
  using namespace krivoshapov;

  const size_t shapeCount = 3;
  Shape* shapes[shapeCount] = {nullptr, nullptr, nullptr};

  try
  {
    point_t rectCenter = {10.0, 10.0};
    shapes[0] = new Rectangle(rectCenter, 5.0, 3.0);

    point_t circleCenter = {5.0, 5.0};
    shapes[1] = new Circle(circleCenter, 2.0);

    point_t rubberCircleCenter = {15.0, 15.0};
    point_t rubberShapeCenter = {14.5, 14.5};
    shapes[2] = new Rubber(rubberCircleCenter, 4.0, rubberShapeCenter);

    std::cout << "=== Before scaling ===\n\n";
    printAllShapesInfo(shapes, shapeCount);

    std::cout << "\n=== Scaling ===\n\n";

    point_t scalePoint = {0.0, 0.0};
    double scaleFactor = 0.0;

    std::cout << "Enter scaling point (x y): ";
    std::cin >> scalePoint.x >> scalePoint.y;

    if (!std::cin)
    {
      std::cerr << "Error: invalid point input\n";
      for (size_t i = 0; i < shapeCount; ++i)
      {
        delete shapes[i];
      }
      return 1;
    }

    std::cout << "Enter scale factor: ";
    std::cin >> scaleFactor;

    if (!std::cin)
    {
      std::cerr << "Error: invalid scale factor input\n";
      for (size_t i = 0; i < shapeCount; ++i)
      {
        delete shapes[i];
      }
      return 1;
    }

    scaleShapes(shapes, shapeCount, scalePoint, scaleFactor);

    std::cout << "\n=== After scaling ===\n\n";
    printAllShapesInfo(shapes, shapeCount);
  }
  catch (const std::exception& error)
  {
    std::cerr << "Error: " << error.what() << "\n";
    for (size_t i = 0; i < shapeCount; ++i)
    {
      delete shapes[i];
    }
    return 2;
  }

  for (size_t i = 0; i < shapeCount; ++i)
  {
    delete shapes[i];
  }

  return 0;
}
