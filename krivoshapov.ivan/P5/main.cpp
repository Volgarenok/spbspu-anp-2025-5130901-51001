#include <iostream>
#include <cmath>

namespace krivoshapov {

    struct point_t {
        double x;
        double y;
    };

    struct rectangle_t {
        double width;
        double height;
        point_t pos;
    };

    class Shape {
        public:
          virtual ~Shape() = default;
          virtual double getArea() const = 0;
          virtual rectangle_t getFrameRect() const = 0;
          virtual void move(double dx, double dy) = 0;
          virtual void move(const point_t& newPos) = 0;
          virtual void scale(double factor) = 0;
    };

    class Circle : public Shape {
        public:
          Circle(const point_t& center, double radius);
          double getArea() const override;
          rectangle_t getFrameRect() const override;
          void move(double dx, double dy) override;
          void move(const point_t& newPos) override;
          void scale(double factor) override;
          point_t getCenter() const;

        private:
          point_t center_;
          double radius_;
    };

    class Rubber : public Shape {
        public:
          Rubber(const point_t& circleCenter, double radius, const point_t& shapeCenter);
          double getArea() const override;
          rectangle_t getFrameRect() const override;
          void move(double dx, double dy) override;
          void move(const point_t& newPos) override;
          void scale(double factor) override;
          point_t getCircleCenter() const;
          point_t getShapeCenter() const;

        private:
          point_t circleCenter_;
          double radius_;
          point_t shapeCenter_;
    };

    class Rectangle : public Shape {
        public:
          Rectangle(const point_t& center, double width, double height);
          double getArea() const override;
          rectangle_t getFrameRect() const override;
          void move(double dx, double dy) override;
          void move(const point_t& newPos) override;
          void scale(double factor) override;
          point_t getCenter() const;

        private:
          point_t center_;
          double width_;
          double height_;
    };

    Circle::Circle(const point_t& center, double radius) :
      center_(center),
      radius_(radius)
    {
        if (radius <= 0.0){
            std::cerr << "Error: circle radius must be positive\n";
        }
    }

    double Circle::getArea() const {
        return M_PI * radius_ * radius_;
    }

    rectangle_t Circle::getFrameRect() const {
        rectangle_t frame;
        frame.width = 2.0 * radius_;
        frame.height = 2.0 * radius_;
        frame.pos = center_;
        return frame;
    }

    void Circle::move(double dx, double dy) {
        center_.x += dx;
        center_.y += dy;
    }

    void Circle::move(const point_t& newPos){
        center_ = newPos;
    }

    void Circle::scale(double factor) {
        if (factor <= 0.0) {
            std::cerr << "Error: scale factor must be positive\n";
            return;
        }
        radius_ *= factor;
    }


    point_t Circle::getCenter() const {
        return center_;
    }
}
