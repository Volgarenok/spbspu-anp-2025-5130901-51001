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
  class Rectangle : Shape {
  private:
    rectangle_t Rect;
  public:
    Rectangle(double width, double height, const point_t& center) {
      if (width <= 0 || height <= 0)
      {
        throw std::invalid_argument("Width and height must be positive");
      }
      Rect.width = width;
      Rect.height = height;
      Rect.pos = center;
    }
    Rectangle(const rectangle_t& rect){
      if (rect.width <= 0 || rect.height <= 0)
      {
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
      if (coef <= 0)
      {
        throw std::invalid_argument("Scale coefficient must be positive");
      }
      Rect.width *= coef;
      Rect.height *= coef;
    }
  };
  class Circle : Shape
  {
  private:
    point_t Center;
    double Radius;
  public:
    ~Circle();
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

}
