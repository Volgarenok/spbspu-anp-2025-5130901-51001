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

    Rubber::Rubber(const point_t& circleCenter, double radius, const point_t& shapeCenter) :
      circleCenter_(circleCenter),
      radius_(radius),
      shapeCenter_(shapeCenter)
    {
        if (radius <= 0.0){
            std::cerr << "Error: rubber radius must be positive\n";
        }

        double dx = shapeCenter.x - circleCenter.x;
        double dy = shapeCenter.y - circleCenter.y;
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist >= radius) {
            std::cerr << "Error: shape center must be inside the circle\n";
        }
    }

    double Rubber::getArea() const {
        return M_PI * radius_ * radius_;
    }

    rectangle_t Rubber::getFrameRect() const {
        rectangle_t frame;
        frame.width = 2.0 * radius_;
        frame.height = 2.0 * radius_;
        frame.pos = circleCenter_;
        return frame;
    }

    void Rubber::move(double dx, double dy){
        circleCenter_.x += dx;
        circleCenter_.y += dy;
        shapeCenter_.x += dx;
        shapeCenter_.y += dy;
    }

    void Rubber::move(const point_t& newPos){
        double dx = newPos.x - shapeCenter_.x;
        double dy = newPos.y - shapeCenter_.y;
        move(dx, dy);
    }

    void Rubber::scale(double factor) {
        if (factor <= 0.0){
            std::cerr << "Error: scale factor must be positive\n";
            return;
        }

        radius_ *= factor;
        double dx = circleCenter_.x - shapeCenter_.x;
        double dy = circleCenter_.y - shapeCenter_.y;
        circleCenter_.x = shapeCenter_.x + dx * factor;
        circleCenter_.y = shapeCenter_.y + dy * factor;
    }

    point_t Rubber::getCircleCenter() const {
        return circleCenter_;
    }

    point_t Rubber::getShapeCenter() const {
        return shapeCenter_;
    }

    Rectangle::Rectangle(const point_t& center, double width, double height) :
      center_(center),
      width_(width),
      height_(height)
    {
        if (width <= 0.0 || height <= 0.0) {
            std::cerr << "Error: rectangle dimensions must be positive\n";
        }
    }

    double Rectangle::getArea() const {
        return width_ * height_;
    }

    rectangle_t Rectangle::getFrameRect() const {
        rectangle_t frame;
        frame.width = width_;
        frame.height = height_;
        frame.pos = center_;
        return frame;
    }

    void Rectangle::move(double dx, double dy) {
        center_.x += dx;
        center_.y += dy;
    }

    void Rectangle::move(const point_t& newPos) {
        center_ = newPos;
    }

    void Rectangle::scale(double factor) {
        if (factor <= 0.0) {
            std::cerr << "Error: scale must be positive\n";
            return;
        }
        width_ *= factor;
        height_ *= factor;
    }

    point_t Rectangle::getCenter() const {
        return center_;
    }

    void sclaleShape(Shape** shapes, size_t count, const point_t& scaleCenter, double factor) {
        if (factor <= 0.0) {
            std::cerr << "Error: scale factor must be positive\n";
            return;
        }

        for (size_t i = 0; i < count; ++i) {
            rectangle_t frame = shapes[i] -> getFrameRect();
            point_t shapeCenter = frame.pos;

            double dx = shapeCenter.x - scaleCenter.x;
            double dy = shapeCenter.y - scaleCenter.y;

            shapes[i] -> move(scaleCenter);
            shapes[i] -> scale(factor);

            point_t newCenter;
            newCenter.x = scaleCenter.x + dx * factor;
            newCenter.y = scaleCenter.y + dy * factor;
            shapes[i] -> move(newCenter);
        }
    }

}
