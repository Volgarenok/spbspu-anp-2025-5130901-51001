#include <iostream>
#include <cmath>

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

    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void move(const point_t &newPos) = 0;

    void scale(double factor)
    {
      if (factor <= 0.0)
      {
        throw "Scale factor must be positive";
      }
      doScale(factor);
    }

  private:
    virtual void doScale(double factor) = 0;
  };

  class Circle final : public Shape
  {
  public:
    Circle(const point_t &center, double radius);
    ~Circle() override = default;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;

  private:
    void doScale(double factor) override;

    point_t center_;
    double radius_;
  };

  class Rectangle final : public Shape
  {
  public:
    Rectangle(const point_t &center, double width, double height);
    ~Rectangle() override = default;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;

  private:
    void doScale(double factor) override;

    point_t center_;
    double width_;
    double height_;
  };

  class Rubber final : public Shape
  {
  public:
    Rubber(const point_t &circleCenter, double radius, const point_t &shapeCenter);
    ~Rubber() override = default;

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(double dx, double dy) override;
    void move(const point_t &newPos) override;

  private:
    void doScale(double factor) override;

    point_t circleCenter_;
    double radius_;
    point_t shapeCenter_;
  };

  Circle::Circle(const point_t &center, double radius) : center_(center),
                                                         radius_(radius)
  {
    if (radius <= 0.0)
    {
      throw "Circle radius must be positive";
    }
  }

  double Circle::getArea() const
  {
    return M_PI * radius_ * radius_;
  }

  rectangle_t Circle::getFrameRect() const
  {
    rectangle_t result;
    result.width = 2.0 * radius_;
    result.height = 2.0 * radius_;
    result.pos = center_;
    return result;
  }

  void Circle::move(double dx, double dy)
  {
    center_.x += dx;
    center_.y += dy;
  }

  void Circle::move(const point_t &newPos)
  {
    center_ = newPos;
  }

  void Circle::doScale(double factor)
  {
    radius_ *= factor;
  }

  Rectangle::Rectangle(const point_t &center, double width, double height) : center_(center),
                                                                             width_(width),
                                                                             height_(height)
  {
    if (width <= 0.0 || height <= 0.0)
    {
      throw "Rectangle dimensions must be positive";
    }
  }

  double Rectangle::getArea() const
  {
    return width_ * height_;
  }

  rectangle_t Rectangle::getFrameRect() const
  {
    rectangle_t result;
    result.width = width_;
    result.height = height_;
    result.pos = center_;
    return result;
  }

  void Rectangle::move(double dx, double dy)
  {
    center_.x += dx;
    center_.y += dy;
  }

  void Rectangle::move(const point_t &newPos)
  {
    center_ = newPos;
  }

  void Rectangle::doScale(double factor)
  {
    width_ *= factor;
    height_ *= factor;
  }

  Rubber::Rubber(const point_t &circleCenter, double radius, const point_t &shapeCenter) : circleCenter_(circleCenter),
                                                                                           radius_(radius),
                                                                                           shapeCenter_(shapeCenter)
  {
    if (radius <= 0.0)
    {
      throw "Rubber radius must be positive";
    }

    double dx = shapeCenter.x - circleCenter.x;
    double dy = shapeCenter.y - circleCenter.y;
    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist >= radius)
    {
      throw "Shape center must be inside the circle";
    }
  }

  double Rubber::getArea() const
  {
    return M_PI * radius_ * radius_;
  }

  rectangle_t Rubber::getFrameRect() const
  {
    rectangle_t result;
    result.width = 2.0 * radius_;
    result.height = 2.0 * radius_;
    result.pos = circleCenter_;
    return result;
  }

  void Rubber::move(double dx, double dy)
  {
    circleCenter_.x += dx;
    circleCenter_.y += dy;
    shapeCenter_.x += dx;
    shapeCenter_.y += dy;
  }

  void Rubber::move(const point_t &newPos)
  {
    double dx = newPos.x - shapeCenter_.x;
    double dy = newPos.y - shapeCenter_.y;
    move(dx, dy);
  }

  void Rubber::doScale(double factor)
  {
    radius_ *= factor;
    double dx = circleCenter_.x - shapeCenter_.x;
    double dy = circleCenter_.y - shapeCenter_.y;
    circleCenter_.x = shapeCenter_.x + dx * factor;
    circleCenter_.y = shapeCenter_.y + dy * factor;
  }

  void scaleShapes(Shape **shapes, size_t count, const point_t &scaleCenter, double factor)
  {
    if (factor <= 0.0)
    {
      throw "Scale factor must be positive";
    }

    for (size_t i = 0; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      point_t shapeCenter = frame.pos;

      double dx = shapeCenter.x - scaleCenter.x;
      double dy = shapeCenter.y - scaleCenter.y;

      shapes[i]->move(scaleCenter);
      shapes[i]->scale(factor);

      point_t newCenter;
      newCenter.x = scaleCenter.x + dx * factor;
      newCenter.y = scaleCenter.y + dy * factor;
      shapes[i]->move(newCenter);
    }
  }

  void printShapeInfo(const Shape &shape, size_t index)
  {
    std::cout << "Shape" << index + 1 << ":\n";
    std::cout << "Area: " << shape.getArea() << "\n";

    rectangle_t frame = shape.getFrameRect();
    std::cout << "Frame rectangle center(" << frame.pos.x << ", " << frame.pos.y << "), ";
    std::cout << "width: " << frame.width << ", height: " << frame.height << "\n";
  }

  rectangle_t getOverallFrameRect(Shape **shapes, size_t count)
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
        minX = left;
      if (right > maxX)
        maxX = right;
      if (bottom < minY)
        minY = bottom;
      if (top > maxY)
        maxY = top;
    }

    rectangle_t result;
    result.width = maxX - minX;
    result.height = maxY - minY;
    result.pos.x = minX + result.width / 2.0;
    result.pos.y = minY + result.height / 2.0;

    return result;
  }

  double getTotalArea(Shape **shapes, size_t count)
  {
    double total = 0.0;
    for (size_t i = 0; i < count; ++i)
    {
      total += shapes[i]->getArea();
    }
    return total;
  }

}

int main()
{
  using namespace krivoshapov;

  const size_t shapeCount = 3;
  Shape *shapes[shapeCount] = {nullptr, nullptr, nullptr};
  bool success = false;

  try
  {
    point_t rectCenter = {10.0, 10.0};
    shapes[0] = new Rectangle(rectCenter, 5.0, 3.0);

    point_t circleCenter = {5.0, 5.0};
    shapes[1] = new Circle(circleCenter, 2.0);

    point_t rubberCircleCenter = {15.0, 15.0};
    point_t rubberShapeCenter = {14.5, 14.5};
    shapes[2] = new Rubber(rubberCircleCenter, 4.0, rubberShapeCenter);

    success = true;

    std::cout << "=== Before scaling ===\n\n";

    for (size_t i = 0; i < shapeCount; ++i)
    {
      printShapeInfo(*shapes[i], i);
    }

    double totalArea = getTotalArea(shapes, shapeCount);
    std::cout << "\nTotal area: " << totalArea << "\n";

    rectangle_t overallFrame = getOverallFrameRect(shapes, shapeCount);
    std::cout << "\nOverall frame rectangle: ";
    std::cout << "center(" << overallFrame.pos.x << ", " << overallFrame.pos.y << "), ";
    std::cout << "width: " << overallFrame.width << ", height: " << overallFrame.height << "\n";

    std::cout << "\n=== Scaling ===\n\n";

    point_t scalePoint;
    double scaleFactor;

    std::cout << "Enter scaling point (x y): ";
    std::cin >> scalePoint.x >> scalePoint.y;

    if (!std::cin)
    {
      std::cerr << "Error: invalid point input\n";
      return 1;
    }

    std::cout << "Enter scale factor: ";
    std::cin >> scaleFactor;

    if (!std::cin)
    {
      std::cerr << "Error: invalid scale factor input\n";
      return 1;
    }

    scaleShapes(shapes, shapeCount, scalePoint, scaleFactor);

    std::cout << "\n=== After scaling ===\n\n";

    for (size_t i = 0; i < shapeCount; ++i)
    {
      printShapeInfo(*shapes[i], i);
    }

    totalArea = getTotalArea(shapes, shapeCount);
    std::cout << "\nTotal area: " << totalArea << "\n";

    rectangle_t newOverallFrame = getOverallFrameRect(shapes, shapeCount);
    std::cout << "\nOverall frame rectangle: ";
    std::cout << "center(" << newOverallFrame.pos.x << ", " << newOverallFrame.pos.y << "), ";
    std::cout << "width: " << newOverallFrame.width << ", height: " << newOverallFrame.height << "\n";
  }
  catch (const char *error)
  {
    std::cerr << "Error: " << error << "\n";
    success = false;
  }

  for (size_t i = 0; i < shapeCount; ++i)
  {
    delete shapes[i];
  }

  if (success)
  {
    return 0;
  }
  else
  {
    return 2;
  }
}
