#include "functions.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace karpenko
{
  void scaleShapes(Shape** shapes, size_t count, const point_t& point, double coefficient)
  {
    for (size_t i = 0; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      double dx = frame.pos.x - point.x;
      double dy = frame.pos.y - point.y;

      shapes[i]->move(point);
      shapes[i]->scale(coefficient);
      shapes[i]->move(-dx * coefficient, -dy * coefficient);
    }
  }

  rectangle_t getOverallFrameRect(const Shape* const* shapes, size_t count)
  {
    const rectangle_t emptyFrame = {0.0, 0.0, {0.0, 0.0}};
    if (count == 0)
    {
      return emptyFrame;
    }

    rectangle_t firstFrame = shapes[0]->getFrameRect();
    double left = firstFrame.pos.x - firstFrame.width / 2.0;
    double right = firstFrame.pos.x + firstFrame.width / 2.0;
    double bottom = firstFrame.pos.y - firstFrame.height / 2.0;
    double top = firstFrame.pos.y + firstFrame.height / 2.0;

    for (size_t i = 1; i < count; ++i)
    {
      rectangle_t frame = shapes[i]->getFrameRect();
      double currentLeft = frame.pos.x - frame.width / 2.0;
      double currentRight = frame.pos.x + frame.width / 2.0;
      double currentBottom = frame.pos.y - frame.height / 2.0;
      double currentTop = frame.pos.y + frame.height / 2.0;

      left = std::min(left, currentLeft);
      right = std::max(right, currentRight);
      bottom = std::min(bottom, currentBottom);
      top = std::max(top, currentTop);
    }

    double width = right - left;
    double height = top - bottom;
    point_t center = {(left + right) / 2.0, (bottom + top) / 2.0};

    rectangle_t result;
    result.width = width;
    result.height = height;
    result.pos = center;
    return result;
  }

  void printShapeInfo(const Shape* shape, size_t index)
  {
    std::cout << "Shape " << index + 1 << ":\n";
    std::cout << "  Area: " << std::fixed << std::setprecision(2) << shape->getArea() << "\n";

    rectangle_t frame = shape->getFrameRect();
    std::cout << "  Frame Rect:\n";
    std::cout << "    Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    std::cout << "    Width: " << frame.width << "\n";
    std::cout << "    Height: " << frame.height << "\n";
  }

  double printAllShapesInfo(const Shape* const* shapes, size_t count, const std::string& title)
  {
    std::cout << "\n" << title << ":\n";

    double totalArea = 0.0;
    for (size_t i = 0; i < count; ++i)
    {
      printShapeInfo(shapes[i], i);
      totalArea += shapes[i]->getArea();

      if (i < count - 1)
      {
        std::cout << "\n";
      }
    }
    return totalArea;
  }

  void printOverallFrameRect(const rectangle_t& frame)
  {
    std::cout << "\nOverall Frame Rect:\n";
    std::cout << "  Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    std::cout << "  Width: " << frame.width << "\n";
    std::cout << "  Height: " << frame.height << "\n";
  }
}
