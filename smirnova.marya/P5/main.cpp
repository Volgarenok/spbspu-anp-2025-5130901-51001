#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace smirnova
{
  struct point_t
  {
    double x, y;
  };

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  struct Shape
  {
    virtual ~Shape() = default;
    virtual double getArea() const noexcept = 0;
    virtual rectangle_t getFrameRect() const noexcept = 0;
    virtual void move(const point_t& p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };

  inline void moveCenter(point_t& center, const double dx, const double dy)
  {
    center.x += dx;
    center.y += dy;
  }

  inline void moveToPoint(point_t& center, const point_t& p)
  {
    center = p;
  }

  inline rectangle_t makeFrameRect(const double w, const double h, const point_t& center) noexcept
  {
    return { w, h, center };
  }

  class Rectangle final : public Shape
  {
  public:
    Rectangle(const double width, const double height, const point_t& center);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& p) override;
    void move(double dx, double dy) override;
    void scale(const double k) override;
  private:
    double width_, height_;
    point_t center_;
  };

  Rectangle::Rectangle(const double width, const double height, const point_t& center) : width_(width), height_(height), center_(center)
  {
    if (width <= 0.0 || height <= 0.0) {
      throw std::invalid_argument("Width and height must be positive");
    }
  }

  double Rectangle::getArea() const noexcept
  {
    return width_ * height_;
  }
  rectangle_t Rectangle::getFrameRect() const noexcept
  {
    return makeFrameRect(width_, height_, center_);
  }
  void Rectangle::move(const point_t& p)
  {
    moveToPoint(center_, p);
  }
  void Rectangle::move(const double dx, const double dy)
  {
    moveCenter(center_, dx, dy);}
  void Rectangle::scale(const double k)
  {
    if (k <= 0.0)
    {
      throw std::invalid_argument("Invalid scale factor");
    }
    width_ *= k;
    height_ *= k;
  }

  class Square final : public Shape
  {
  public:
    Square(const double side, const point_t& center);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& p) override;
    void move(double dx, double dy) override;
    void scale(const double k) override;
  private:
    double side_;
    point_t center_;
  };

  Square::Square(const double side, const point_t& center) : side_(side), center_(center)
  {
    if (side <= 0.0)
      throw std::invalid_argument("Side must be positive");
  }

  double Square::getArea() const noexcept
  {
    return side_ * side_;
  }
  rectangle_t Square::getFrameRect() const noexcept
  {
    return makeFrameRect(side_, side_, center_);
  }
  void Square::move(const point_t& p)
  {
    moveToPoint(center_, p);
  }
  void Square::move(const double dx, const double dy)
  {
    moveCenter(center_, dx, dy);
  }
  void Square::scale(const double k)
  {
    if (k <= 0.0)
    {
      throw std::invalid_argument("Invalid scale factor");
    }
    side_ *= k;
  }

  class Triangle final : public Shape
  {
	public:
    Triangle(const point_t a, const point_t b, const point_t c);
    double getArea() const noexcept override;
    rectangle_t getFrameRect() const noexcept override;
    void move(const point_t& p) override;
    void move(double dx, double dy) override;
    void scale(const double k) override;
  private:
    point_t a_, b_, c_;
  };

  Triangle::Triangle(const point_t a, const point_t b, const point_t c)
    : a_(a), b_(b), c_(c)
    {
    if (getArea() <= 0.0)
    {
      throw std::invalid_argument("Points do not form a valid triangle");
    }
  }

  double Triangle::getArea() const noexcept
  {
    return 0.5 * std::abs(a_.x*(b_.y - c_.y) + b_.x*(c_.y - a_.y) + c_.x*(a_.y - b_.y));
  }

  rectangle_t Triangle::getFrameRect() const noexcept
  {
    const double minX = std::min({a_.x, b_.x, c_.x});
    const double maxX = std::max({a_.x, b_.x, c_.x});
    const double minY = std::min({a_.y, b_.y, c_.y});
    const double maxY = std::max({a_.y, b_.y, c_.y});
    return makeFrameRect(maxX - minX, maxY - minY, { (minX + maxX)/2.0, (minY + maxY)/2.0 });
  }

  void Triangle::move(const point_t& p)
  {
    rectangle_t frame = getFrameRect();
    double dx = p.x - frame.pos.x;
    double dy = p.y - frame.pos.y;
    move(dx, dy);
  }

  void Triangle::move(const double dx, const double dy)
  {
    moveCenter(a_, dx, dy);
    moveCenter(b_, dx, dy);
    moveCenter(c_, dx, dy);
  }

  void Triangle::scale(const double k)
  {
    if (k <= 0.0)
    {
      throw std::invalid_argument("Invalid scale factor");
    }
    rectangle_t frame = getFrameRect();
    point_t center = frame.pos;
    a_.x = center.x + (a_.x - center.x) * k; a_.y = center.y + (a_.y - center.y) * k;
    b_.x = center.x + (b_.x - center.x) * k; b_.y = center.y + (b_.y - center.y) * k;
    c_.x = center.x + (c_.x - center.x) * k; c_.y = center.y + (c_.y - center.y) * k;
  }

  rectangle_t getTotalFrameRect(Shape* shapes[], int count) noexcept
  {
    if (count <= 0)
    {
      return {0.0, 0.0, {0.0, 0.0}};
    }
    rectangle_t total = shapes[0]->getFrameRect();
    for (int i = 1; i < count; ++i)
    {
      rectangle_t f = shapes[i]->getFrameRect();
      double minX = std::min(total.pos.x - total.width/2, f.pos.x - f.width/2);
      double maxX = std::max(total.pos.x + total.width/2, f.pos.x + f.width/2);
      double minY = std::min(total.pos.y - total.height/2, f.pos.y - f.height/2);
      double maxY = std::max(total.pos.y + total.height/2, f.pos.y + f.height/2);
      total.width = maxX - minX;
      total.height = maxY - minY;
      total.pos = {(minX + maxX)/2.0, (minY + maxY)/2.0};
    }
    return total;
  }

}

int main()
{
  using namespace smirnova;

  try
  {
    Rectangle rect(4.0, 2.0, {0.0, 0.0});
    Square sq(3.0, {5.0, 5.0});
    Triangle tri({0.0, 0.0}, {4.0, 0.0}, {2.0, 3.0});
    Shape* shapes[3] = { &rect, &tri, &sq };
    const char* names[3] = { "Прямоугольник", "Треугольник",  "Квадрат" };
    std::cout << "До масштабирования:\n";
    double totalArea = 0.0;
    for (int i = 0; i < 3; ++i)
    {
      double area = shapes[i]->getArea();
      rectangle_t frame = shapes[i]->getFrameRect();
      totalArea += area;
      std::cout << names[i] << ": площадь = " << area << "\n";
      std::cout << "Ограничивающий прямоугольник: ширина = " << frame.width <<", высота = " << frame.height 
      << ", центр = (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    }
    std::cout << "Суммарная площадь: " << totalArea << "\n";
    rectangle_t totalFrame = getTotalFrameRect(shapes, 3);
    std::cout << "Общий ограничивающий прямоугольник: ширина = " << totalFrame.width << ", высота = " << totalFrame.height << ", центр = (" << totalFrame.pos.x << ", " << totalFrame.pos.y << ")\n";

    point_t scalePoint;
    double k;
    std::cout << "Введите точку масштабирования (x y): ";
    if (!(std::cin >> scalePoint.x >> scalePoint.y))
    {
      std::cerr << "Ошибка ввода: некорректная точка масштабирования\n";
      return 1;
    }
    std::cout << "Введите коэффициент масштабирования: ";
    if (!(std::cin >> k) || k <= 0.0)
    {
      std::cerr << "Ошибка ввода: некорректный коэффициент масштабирования\n";
      return 1;
    }
    for (int i = 0; i < 3; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      double dx = scalePoint.x - frame.pos.x;
      double dy = scalePoint.y - frame.pos.y;
      shapes[i]->move(dx, dy);
      shapes[i]->scale(k);
      shapes[i]->move(-dx, -dy);
    }

    std::cout << "\nПосле масштабирования:\n";
    totalArea = 0.0;
    for (int i = 0; i < 3; ++i)
    {
      double area = shapes[i]->getArea();
      rectangle_t frame = shapes[i]->getFrameRect();
      totalArea += area;
      std::cout << names[i] << ": площадь = " << area << "\n";
      std::cout << "Ограничивающий прямоугольник: ширина = " << frame.width << ", высота = " << frame.height << ", центр = (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    }
    std::cout << "Суммарная площадь: " << totalArea << "\n";
    totalFrame = getTotalFrameRect(shapes, 3);
    std::cout << "Общий ограничивающий прямоугольник: ширина = " << totalFrame.width << ", высота = " << totalFrame.height << ", центр = (" << totalFrame.pos.x << ", " << totalFrame.pos.y << ")\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << "Ошибка: " << e.what() << "\n";
    return 1;
  }

  return 0;
}