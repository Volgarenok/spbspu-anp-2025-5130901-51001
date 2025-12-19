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
        virtual void scale(double factor) = 0;
    };

    class Circle : public Shape
    {
    public:
        Circle(const point_t &center, double radius);
        double getArea() const override;
        rectangle_t getFrameRect() const override;
        void move(double dx, double dy) override;
        void move(const point_t &newPos) override;
        void scale(double factor) override;
        point_t getCenter() const;

    private:
        point_t center_;
        double radius_;
    };

    class Rubber : public Shape
    {
    public:
        Rubber(const point_t &circleCenter, double radius, const point_t &shapeCenter);
        double getArea() const override;
        rectangle_t getFrameRect() const override;
        void move(double dx, double dy) override;
        void move(const point_t &newPos) override;
        void scale(double factor) override;
        point_t getCircleCenter() const;
        point_t getShapeCenter() const;

    private:
        point_t circleCenter_;
        double radius_;
        point_t shapeCenter_;
    };

    class Rectangle : public Shape
    {
    public:
        Rectangle(const point_t &center, double width, double height);
        double getArea() const override;
        rectangle_t getFrameRect() const override;
        void move(double dx, double dy) override;
        void move(const point_t &newPos) override;
        void scale(double factor) override;
        point_t getCenter() const;

    private:
        point_t center_;
        double width_;
        double height_;
    };

    Circle::Circle(const point_t &center, double radius) : center_(center),
                                                           radius_(radius)
    {
        if (radius <= 0.0)
        {
            std::cerr << "Error: circle radius must be positive\n";
        }
    }

    double Circle::getArea() const
    {
        return M_PI * radius_ * radius_;
    }

    rectangle_t Circle::getFrameRect() const
    {
        rectangle_t frame;
        frame.width = 2.0 * radius_;
        frame.height = 2.0 * radius_;
        frame.pos = center_;
        return frame;
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

    void Circle::scale(double factor)
    {
        if (factor <= 0.0)
        {
            std::cerr << "Error: scale factor must be positive\n";
            return;
        }
        radius_ *= factor;
    }

    point_t Circle::getCenter() const
    {
        return center_;
    }

    Rubber::Rubber(const point_t &circleCenter, double radius, const point_t &shapeCenter) : circleCenter_(circleCenter),
                                                                                             radius_(radius),
                                                                                             shapeCenter_(shapeCenter)
    {
        if (radius <= 0.0)
        {
            std::cerr << "Error: rubber radius must be positive\n";
        }

        double dx = shapeCenter.x - circleCenter.x;
        double dy = shapeCenter.y - circleCenter.y;
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist >= radius)
        {
            std::cerr << "Error: shape center must be inside the circle\n";
        }
    }

    double Rubber::getArea() const
    {
        return M_PI * radius_ * radius_;
    }

    rectangle_t Rubber::getFrameRect() const
    {
        rectangle_t frame;
        frame.width = 2.0 * radius_;
        frame.height = 2.0 * radius_;
        frame.pos = circleCenter_;
        return frame;
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

    void Rubber::scale(double factor)
    {
        if (factor <= 0.0)
        {
            std::cerr << "Error: scale factor must be positive\n";
            return;
        }

        radius_ *= factor;
        double dx = circleCenter_.x - shapeCenter_.x;
        double dy = circleCenter_.y - shapeCenter_.y;
        circleCenter_.x = shapeCenter_.x + dx * factor;
        circleCenter_.y = shapeCenter_.y + dy * factor;
    }

    point_t Rubber::getCircleCenter() const
    {
        return circleCenter_;
    }

    point_t Rubber::getShapeCenter() const
    {
        return shapeCenter_;
    }

    Rectangle::Rectangle(const point_t &center, double width, double height) : center_(center),
                                                                               width_(width),
                                                                               height_(height)
    {
        if (width <= 0.0 || height <= 0.0)
        {
            std::cerr << "Error: rectangle dimensions must be positive\n";
        }
    }

    double Rectangle::getArea() const
    {
        return width_ * height_;
    }

    rectangle_t Rectangle::getFrameRect() const
    {
        rectangle_t frame;
        frame.width = width_;
        frame.height = height_;
        frame.pos = center_;
        return frame;
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

    void Rectangle::scale(double factor)
    {
        if (factor <= 0.0)
        {
            std::cerr << "Error: scale must be positive\n";
            return;
        }
        width_ *= factor;
        height_ *= factor;
    }

    point_t Rectangle::getCenter() const
    {
        return center_;
    }

    void ScaleShapes(Shape **shapes, size_t count, const point_t &scaleCenter, double factor)
    {
        if (factor <= 0.0)
        {
            std::cerr << "Error: scale factor must be positive\n";
            return;
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
        std::cout << "Area:" << shape.getArea() << "\n";

        rectangle_t frame = shape.getFrameRect();
        std::cout << "Frame rectangle center(" << frame.pos.x << ", " << frame.pos.y << "), width:"
                  << frame.width << ", height:" << frame.height << "\n";
    }

    rectangle_t getOverallFrameRect(Shape **shapes, size_t count)
    {
        rectangle_t result;

        if (count == 0)
        {
            result.width = 0.0;
            result.height = 0.0;
            result.pos.x = 0.0;
            result.pos.y = 0.0;
            return result;
        }

        rectangle_t firstFrame = shapes[0]->getFrameRect();
        double minX = firstFrame.pos.x - firstFrame.width / 2;
        double maxX = firstFrame.pos.x + firstFrame.width / 2;
        double minY = firstFrame.pos.y - firstFrame.height / 2;
        double maxY = firstFrame.pos.y + firstFrame.height / 2;

        for (size_t i = 1; i < count; ++i)
        {
            rectangle_t frame = shapes[i]->getFrameRect();

            double left = frame.pos.x - frame.width / 2;
            double right = frame.pos.x + frame.width / 2;
            double bottom = frame.pos.y - frame.height / 2;
            double top = frame.pos.y + frame.height / 2;

            if (left < minX)
            {
                minX = left;
            }
            if (right > maxX)
            {
                maxX = right;
            }
            if (bottom < minY)
            {
                minY = bottom;
            }
            if (top > maxY)
            {
                maxY = top;
            }
        }
        result.width = maxX - minX;
        result.height = maxY - minY;
        result.pos.x = minX + result.width / 2;
        result.pos.y = minY + result.height / 2;

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
    Shape *shapes[shapeCount];

    point_t rectCenter = {10.0, 10.0};
    shapes[0] = new Rectangle(rectCenter, 5.0, 3.0);

    point_t circleCenter = {5.0, 5.0};
    shapes[1] = new Circle(circleCenter, 2.0);

    point_t rubberCircleCenter = {15.0, 15.0};
    point_t rubberShapeCenter = {14.5, 14.5};
    shapes[2] = new Rubber(rubberCircleCenter, 4.0, rubberShapeCenter);

    std::cout << "=== Before scaling ===\n\n";

    for (size_t i = 0; i < shapeCount; ++i)
    {
        printShapeInfo(*shapes[i], i);
    }

    double totalArea = getTotalArea(shapes, shapeCount);
    std::cout << "\n Total area: " << totalArea << "\n";

    rectangle_t overallFrame = getOverallFrameRect(shapes, shapeCount);
    std::cout << "\n Overall frame rectangle: center(" << overallFrame.pos.x << ", "
              << overallFrame.pos.y << "), width: " << overallFrame.width << ", height: "
              << overallFrame.height << "\n";

    std::cout << "\n === Scaling ===\n\n";

    point_t scalePoint;
    double scaleFactor;

    std::cout << "Enter scaling point (x y):";
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

    std::cout << "Enter scale factor:";
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

    ScaleShapes(shapes, shapeCount, scalePoint, scaleFactor);

    std::cout << "\n === After scaling ===\n\n";

    for (size_t i = 0; i < shapeCount; ++i)
    {
        printShapeInfo(*shapes[i], i);
    }

    totalArea = getTotalArea(shapes, shapeCount);
    std::cout << "\n Total area: " << totalArea << "\n";

    std::cout << "\n  Overall frame rectangle: center("
              << overallFrame.pos.x << ", " << overallFrame.pos.y
              << "), width: " << overallFrame.width
              << ", height: " << overallFrame.height << "\n";

    for (size_t i = 0; i < shapeCount; ++i)
    {
        delete shapes[i];
    }

    return 0;
}
