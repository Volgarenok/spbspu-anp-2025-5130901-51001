#include <iostream>
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <cmath>
namespace smirnova
{
  struct point_t {
    double x, y;
  };
  struct rectangle_t {
    double width, height;
    point_t pos;
  };
}
  struct Shape
  {
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(const point_t& p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };

  class Rectangle final : public Shape {
  private:
    double width_{0.0}, height_{0.0};
    point_t pos_{0.0, 0.0};
  public:
    Rectangle() noexcept = default;
    Rectangle(double width, double height, const point_t& pos) :
    width_(width), height_(height), pos_(pos)
    {
      if (width <= 0.0 || height <= 0.0) {
        throw std::invalid_argument("Incorrect coordinates of a point\n");
      }
    }

    double getArea() const noexcept override {
      return width_ * height_;
    }

    rectangle_t getFrameRect() const noexcept override {
      return {width_, height_, pos_};
    }

    void move(const point_t& p) noexcept override {
      pos_ = p;
    }

    void move(double dx, double dy) noexcept override {
      pos_.x += dx;
      pos_.y += dy;
    }

    void scale(double k) override {
      if (k <= 0.0) {
        throw std::invalid_argument("Incorrect coefficient\n");
      }
      width_ *= k;
      height_ *= k;
    }
  };
  class Triangle final : public Shape {
  private:
    double base_{0.0}, height_{0.0};
    point_t center_{0.0, 0.0};
  protected:
    void scaleImpl(double k) override {
      base_ *= k;
      height_ *= k;
    }
  public:
    Triangle() noexcept = default;
    Triangle(double base, double height, const point_t& center) :
      base_(base), height_(height), center_(center)
    {
      if (base <= 0.0 || height <= 0.0) {
        throw std::invalid_argument("Incorrect dimensions\n");
      }
    }

    double getArea() const noexcept override {
      return 0.5 * base_ * height_;
    }

    rectangle_t getFrameRect() const noexcept override {
      return {base_, height_, center_};
    }

    void move(const point_t& p) noexcept override {
      center_ = p;
    }

    void move(double dx, double dy) noexcept override {
      center_.x += dx;
      center_.y += dy;
    }
  };
}
inline void validateTriangle(const smi)