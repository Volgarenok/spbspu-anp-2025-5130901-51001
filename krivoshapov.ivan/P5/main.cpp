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

}
