#include <iostream>
#include <algorithm>

namespace losev {
  float const Pi = 3.1415;
  struct point_t {
    double x, y;
  };
  struct rectangle_t {
    double width, height;
    point_t pos;
  };
  class Shape {
  public:
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& point) noexcept = 0;
    virtual void move(double dx, double dy) noexcept = 0;
    void scale(double coef);
    virtual void unsafeScale(double coef) = 0;
  };
  class Rectangle final: public Shape {
  public:
    Rectangle(double width, double height, const point_t& center);
    Rectangle(const rectangle_t& rect);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void unsafeScale(double coef) override;
  private:
    rectangle_t rect_;
  };
  class Circle final: public Shape {
  public:
    Circle(const point_t& center, double radius);
    Circle(double x, double y, double radius);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void unsafeScale(double coef) override;
  private:
    point_t center_;
    double radius_;
  };
  class Ring final: public Shape {
  public:
    Ring(const point_t& center, double outerR, double innerR);
    Ring(double x, double y, double outerR, double innerR);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& point) noexcept override;
    void move(double dx, double dy) noexcept override;
    void unsafeScale(double coef) override;
  private:
    point_t center_;
    double outerRadius_;
    double innerRadius_;
  };
  void unsafeScaleFromPoint(Shape& shape, const point_t& point, double coef);
  rectangle_t getOverallFrameRect(const Shape* const* shapes, size_t count);
  void printShapesInfo(const Shape* const* shapes, size_t count);
  void printRectangleInfo(const rectangle_t& rect);
}
void losev::Shape::scale(double coef)
{
  if (coef <= 0) {
    throw std::invalid_argument("Scale coefficient must be positive");
  }
  return unsafeScale(coef);
}
losev::Rectangle::Rectangle(double width, double height, const point_t& center):
  rect_{width, height, center}
{
  if (width <= 0 || height <= 0) {
    throw std::invalid_argument("Width and height must be positive");
  }
}
losev::Rectangle::Rectangle(const rectangle_t& rect):
  rect_(rect)
{
  if (rect.width <= 0 || rect.height <= 0) {
    throw std::invalid_argument("Width and height must be positive");
  }
}
double losev::Rectangle::getArea() const noexcept
{
  return rect_.width * rect_.height;
}
losev::rectangle_t losev::Rectangle::getFrameRect() const noexcept
{
  return rect_;
}
void losev::Rectangle::move(const point_t& point) noexcept
{
  rect_.pos = point;
}
void losev::Rectangle::move(double dx, double dy) noexcept
{
  rect_.pos.x += dx;
  rect_.pos.y += dy;
}
void losev::Rectangle::unsafeScale(double coef)
{
  rect_.width *= coef;
  rect_.height *= coef;
}
losev::Circle::Circle(const point_t& center, double radius):
  center_(center),
  radius_(radius)
{
  if (radius <= 0) {
    throw std::invalid_argument("Radius must be positive");
  }
}
losev::Circle::Circle(double x, double y, double radius):
  center_{x, y},
  radius_(radius)
{
  if (radius <= 0) {
    throw std::invalid_argument("Radius must be positive");
  }
}
double losev::Circle::getArea() const noexcept
{
  return losev::Pi * radius_ * radius_;
}
losev::rectangle_t losev::Circle::getFrameRect() const noexcept
{
  rectangle_t frameRect;
  frameRect.width = 2.0 * radius_;
  frameRect.height = 2.0 * radius_;
  frameRect.pos = center_;
  return frameRect;
}
void losev::Circle::move(const point_t& point) noexcept
{
  center_ = point;
}
void losev::Circle::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void losev::Circle::unsafeScale(double coef)
{
  radius_ *= coef;
}
losev::Ring::Ring(const point_t& center, double outerR, double innerR):
  center_(center),
  outerRadius_(outerR),
  innerRadius_(innerR)
{
  if (outerR <= 0 || innerR <= 0) {
    throw std::invalid_argument("Radii must be positive");
  }
  if (innerR >= outerR) {
    throw std::invalid_argument("Inner radius must be less than outer radius");
  }
}
losev::Ring::Ring(double x, double y, double outerR, double innerR):
  center_{x, y},
  outerRadius_(outerR),
  innerRadius_(innerR)
{
  if (outerR <= 0 || innerR <= 0) {
    throw std::invalid_argument("Radii must be positive");
  }
  if (innerR >= outerR) {
    throw std::invalid_argument("Inner radius must be less than outer radius");
  }
}
double losev::Ring::getArea() const noexcept
{
  return losev::Pi * (outerRadius_ * outerRadius_ - innerRadius_ * innerRadius_);
}
losev::rectangle_t losev::Ring::getFrameRect() const noexcept
{
  rectangle_t frameRect;
  frameRect.width = 2.0 * outerRadius_;
  frameRect.height = 2.0 * outerRadius_;
  frameRect.pos = center_;
  return frameRect;
}
void losev::Ring::move(const point_t& point) noexcept
{
  center_ = point;
}
void losev::Ring::move(double dx, double dy) noexcept
{
  center_.x += dx;
  center_.y += dy;
}
void losev::Ring::unsafeScale(double coef)
{
  outerRadius_ *= coef;
  innerRadius_ *= coef;
}

int main()
{
  using namespace losev;
  Shape** shapes = nullptr;
  try {
    shapes = new Shape*[6]();
    shapes[0] = new Rectangle(4.0, 3.0, {1.0, 2.0});
    shapes[1] = new Rectangle(rectangle_t{5.0, 2.0, {3.0, 4.0}});
    shapes[2] = new Circle({0.0, 0.0}, 2.0);
    shapes[3] = new Circle(5.0, 5.0, 1.5);
    shapes[4] = new Ring({2.0, -1.0}, 3.0, 1.0);
    shapes[5] = new Ring(-2.0, -2.0, 2.5, 0.5);
    std::cout << "=== BEFORE scaling ===\n";
    printShapesInfo(shapes, 6);
    point_t scalePoint = {0.0, 0.0};;
    double scaleCoef = 1.0;
    std::cout << "\n=== Scaling parameters input ===\n";
    std::cout << "Enter scaling point coordinates (x y): ";
    if (!(std::cin >> scalePoint.x >> scalePoint.y)) {
      std::cerr << "Error: invalid coordinate input!\n";
      if (shapes != nullptr) {
        for (size_t i = 0; i < 6; ++i) {
          delete shapes[i];
        }
        delete[] shapes;
      }
      return 1;
    }
    std::cout << "Enter scaling coefficient (positive number): ";
    if (!(std::cin >> scaleCoef)) {
      std::cerr << "Error: invalid coefficient input!\n";
      if (shapes != nullptr) {
        for (size_t i = 0; i < 6; ++i) {
          delete shapes[i];
        }
        delete[] shapes;
      }
      return 1;
    }
    if (scaleCoef <= 0) {
      std::cerr << "Error: scaling coefficient must be positive!" << "\n";
      if (shapes != nullptr) {
        for (size_t i = 0; i < 6; ++i) {
          delete shapes[i];
        }
        delete[] shapes;
      }
      return 1;
    }
    for (size_t i = 0; i < 6; ++i) {
      unsafeScaleFromPoint(*shapes[i], scalePoint, scaleCoef);
    }
    std::cout << "\n=== AFTER scaling ===" << "\n";
    printShapesInfo(shapes, 6);
    std::cout << "\nScaling performed relative to point ("
     << scalePoint.x << ", " << scalePoint.y << ") with coefficient "
     << scaleCoef << "\n";
    for (size_t i = 0; i < 6; ++i) {
      delete shapes[i];
    }
    delete[] shapes;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    if (shapes != nullptr) {
      for (size_t i = 0; i < 6; ++i) {
        delete shapes[i];
      }
      delete[] shapes;
    }
    return 1;
  }
  return 0;
}

void losev::printRectangleInfo(const rectangle_t& rect)
{
  std::cout << "  Center: (" << rect.pos.x << ", " << rect.pos.y << ")\n";
  std::cout << "  Width: " << rect.width << ", Height: " << rect.height << "\n";
}

void losev::unsafeScaleFromPoint(Shape& shape, const point_t& point, double coef)
{
  rectangle_t frame = shape.getFrameRect();
  point_t center = frame.pos;
  double dx = point.x - center.x;
  double dy = point.y - center.y;
  shape.move(point);
  shape.unsafeScale(coef);
  shape.move(-dx * coef, -dy * coef);
}

losev::rectangle_t losev::getOverallFrameRect(const Shape* const* shapes, size_t count)
{
  if (count == 0) {
    return {0, 0, {0, 0}};
  }
  rectangle_t first = shapes[0]->getFrameRect();
  double minX = first.pos.x - first.width / 2;
  double maxX = first.pos.x + first.width / 2;
  double minY = first.pos.y - first.height / 2;
  double maxY = first.pos.y + first.height / 2;
  for (size_t i = 1; i < count; ++i) {
    rectangle_t rect = shapes[i]->getFrameRect();
    double left = rect.pos.x - rect.width / 2;
    double right = rect.pos.x + rect.width / 2;
    double bottom = rect.pos.y - rect.height / 2;
    double top = rect.pos.y + rect.height / 2;
    minX = std::min(minX, left);
    maxX = std::max(maxX, right);
    minY = std::min(minY, bottom);
    maxY = std::max(maxY, top);
  }
  rectangle_t overall;
  overall.width = maxX - minX;
  overall.height = maxY - minY;
  overall.pos.x = (minX + maxX) / 2;
  overall.pos.y = (minY + maxY) / 2;
  return overall;
}
void losev::printShapesInfo(const Shape* const* shapes, size_t count)
{
  double totalArea = 0.0;
  for (size_t i = 0; i < count; ++i) {
    double area = shapes[i]->getArea();
    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << "Shape " << (i + 1) << ":\n";
    std::cout << "  Area: " << area << "\n";
    std::cout << "  Frame rectangle:\n";
    printRectangleInfo(frame);
    totalArea += area;
  }
  std::cout << "\nTotal area of all shapes: " << totalArea << "\n";
  rectangle_t overallFrame = getOverallFrameRect(shapes, count);
  std::cout << "\nOverall frame rectangle:" << "\n";
  printRectangleInfo(overallFrame);
}
