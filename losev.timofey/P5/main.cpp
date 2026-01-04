#include<iostream>
namespace losev {
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
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(const point_t & point) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
  };
  class Rectangle : public Shape {
  private:
    rectangle_t Rect;
  public:
    Rectangle(double width, double height, const point_t& center) {
      if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Width and height must be positive");
      }
      Rect.width = width;
      Rect.height = height;
      Rect.pos = center;
    }
    Rectangle(const rectangle_t& rect) {
      if (rect.width <= 0 || rect.height <= 0) {
        throw std::invalid_argument("Width and height must be positive");
      }
      Rect = rect;
    }
    double getArea() const override
    {
      return Rect.width * Rect.height;
    }
    rectangle_t getFrameRect() const override
    {
      return Rect;
    }
    void move(const point_t & point) override
    {
      Rect.pos = point;
    }
    void move(double dx, double dy) override
    {
      Rect.pos.x += dx;
      Rect.pos.y += dy;
    }
    void scale(double coef) override
    {
      if (coef <= 0) {
        throw std::invalid_argument("Scale coefficient must be positive");
      }
      Rect.width *= coef;
      Rect.height *= coef;
    }
  };
  class Circle : public Shape
  {
  private:
    point_t Center;
    double Radius;
  public:
    Circle(const point_t & center, double radius) {
      if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
      }
      Center = center;
      Radius = radius;
    }
    Circle(double x, double y, double radius) {
      if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
      }
      Center.x = x;
      Center.y = y;
      Radius = radius;
    }
    double getArea() const override {
      return 3.1415 * Radius * Radius;
    }
    rectangle_t getFrameRect() const override {
      rectangle_t frameRect;
      frameRect.width = 2.0 * Radius;
      frameRect.height = 2.0 * Radius;
      frameRect.pos = Center;
      return frameRect;
    }
    void move(const point_t & point) override {
      Center = point;
    }
    void move(double dx, double dy) override {
      Center.x += dx;
      Center.y += dy;
    } 
    void scale(double coef) override {
      if (coef <= 0) {
        throw std::invalid_argument("Scale coefficient must be positive");
      }
      Radius *= coef;
    }
  };
  class Ring : public Shape
  {
  private:
    point_t Center;
    double outerRadius;
    double innerRadius;
  public:
    Ring(const point_t& center, double outerR, double innerR) {
      if (outerR <= 0 || innerR <= 0) {
        throw std::invalid_argument("Radii must be positive");
      }
      if (innerR >= outerR) {
        throw std::invalid_argument("Inner radius must be less than outer radius");
      }
      Center = center;
      outerRadius = outerR;
      innerRadius = innerR;
    }
    Ring(double x, double y, double outerR, double innerR) {
      if (outerR <= 0 || innerR <= 0) {
        throw std::invalid_argument("Radii must be positive");
      }
      if (innerR >= outerR) {
        throw std::invalid_argument("Inner radius must be less than outer radius");
      }
      Center.x = x;
      Center.y = y;
      outerRadius = outerR;
      innerRadius = innerR;
    }
    double getArea() const override {
      return 3.1415 * (outerRadius * outerRadius - innerRadius * innerRadius);
    }
    rectangle_t getFrameRect() const override {
      rectangle_t frameRect;
      frameRect.width = 2.0 * outerRadius;
      frameRect.height = 2.0 * outerRadius;
      frameRect.pos = Center;
      return frameRect;
    }
    void move(const point_t& point) override {
      Center = point;
    }
    void move(double dx, double dy) override {
      Center.x += dx;
      Center.y += dy;
    }
    void scale(double coef) override {
      if (coef <= 0) {
        throw std::invalid_argument("Scale coefficient must be positive");
      }
      outerRadius *= coef;
      innerRadius *= coef;
    }
    point_t getCenter() const {
      return Center;
    }
    double getOuterRadius() const {
      return outerRadius;
    }
    double getInnerRadius() const {
      return innerRadius;
    }
  };
  void scaleFromPoint(Shape& shape, const losev::point_t& point, double coef);
  rectangle_t getOverallFrameRect(Shape** shapes, size_t count);
  void printShapesInfo(Shape** shapes, size_t count);
}
int main()
{
  using namespace losev;
  try {
    Shape** shapes = new Shape*[6];
    shapes[0] = new Rectangle(4.0, 3.0, {1.0, 2.0});
    shapes[1] = new Rectangle(rectangle_t{5.0, 2.0, {3.0, 4.0}});
    shapes[2] = new Circle({0.0, 0.0}, 2.0);
    shapes[3] = new Circle(5.0, 5.0, 1.5);
    shapes[4] = new Ring({2.0, -1.0}, 3.0, 1.0);
    shapes[5] = new Ring(-2.0, -2.0, 2.5, 0.5);
    std::cout << "=== BEFORE scaling ===\n";
    printShapesInfo(shapes, 6);
    point_t scalePoint;
    double scaleCoef;
    std::cout << "\n=== Scaling parameters input ===\n";
    std::cout << "Enter scaling point coordinates (x y): ";
    if (!(std::cin >> scalePoint.x >> scalePoint.y)) {
      std::cerr << "Error: invalid coordinate input!\n";
      for (size_t i = 0; i < 6; ++i) {
        delete shapes[i];
      }
      delete[] shapes;
      return 1;
    }
    std::cout << "Enter scaling coefficient (positive number): ";
    if (!(std::cin >> scaleCoef)) {
      std::cerr << "Error: invalid coefficient input!\n";
      for (size_t i = 0; i < 6; ++i) {
        delete shapes[i];
      }
      delete[] shapes;
      return 1;
    }
    if (scaleCoef <= 0) {
      std::cerr << "Error: scaling coefficient must be positive!" << "\n";
      for (size_t i = 0; i < 6; ++i) {
        delete shapes[i];
      }
      delete[] shapes;
      return 1;
    }
    for (size_t i = 0; i < 6; ++i) {
      scaleFromPoint(*shapes[i], scalePoint, scaleCoef);
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
    return 1;
  }
  return 0;
}
void losev::scaleFromPoint(Shape& shape, const losev::point_t& point, double coef) {
losev::rectangle_t frame = shape.getFrameRect();
  losev::point_t center = frame.pos;
  double dx = point.x - center.x;
  double dy = point.y - center.y;
  shape.move(point);
  shape.scale(coef);
  shape.move(-dx * coef, -dy * coef);
}
losev::rectangle_t losev::getOverallFrameRect(Shape** shapes, size_t count) {
  if (count == 0) {
    losev::rectangle_t emptyRect = {0, 0, {0, 0}};
    return emptyRect;
  }
  losev::rectangle_t first = shapes[0]->getFrameRect();
  double minX = first.pos.x - first.width / 2;
  double maxX = first.pos.x + first.width / 2;
  double minY = first.pos.y - first.height / 2;
  double maxY = first.pos.y + first.height / 2;
  for (size_t i = 1; i < count; ++i) {
    losev::rectangle_t rect = shapes[i]->getFrameRect();
    double left = rect.pos.x - rect.width / 2;
    double right = rect.pos.x + rect.width / 2;
    double bottom = rect.pos.y - rect.height / 2;
    double top = rect.pos.y + rect.height / 2;
    if (left < minX) {
      minX = left;
    }
    if (right > maxX) {
      maxX = right;
    }
    if (bottom < minY) {
      minY = bottom;
    }
    if (top > maxY) {
      maxY = top;
    }
  }
  losev::rectangle_t overall;
  overall.width = maxX - minX;
  overall.height = maxY - minY;
  overall.pos.x = (minX + maxX) / 2;
  overall.pos.y = (minY + maxY) / 2;
  return overall;
}
void losev::printShapesInfo(losev::Shape** shapes, size_t count) {
  double totalArea = 0.0;
  for (size_t i = 0; i < count; ++i) {
    double area = shapes[i]->getArea();
    losev::rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << "Shape " << (i + 1) << ":\n";
    std::cout << "  Area: " << area << "\n";
    std::cout << "  Frame rectangle:\n";
    std::cout << "    Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    std::cout << "    Width: " << frame.width << ", Height: " << frame.height << "\n";
    totalArea += area;
  }
  std::cout << "\nTotal area of all shapes: " << totalArea << "\n";
  losev::rectangle_t overallFrame = losev::getOverallFrameRect(shapes, count);
  std::cout << "\nOverall frame rectangle:" << "\n";
  std::cout << "  Center: (" << overallFrame.pos.x << ", " << overallFrame.pos.y << ")" << "\n";
  std::cout << "  Width: " << overallFrame.width << ", Height: " << overallFrame.height << "\n";
}
