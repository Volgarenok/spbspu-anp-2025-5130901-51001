#include "functions.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

void karpenko::scaleShapes(Shape** shapes, size_t count, const point_t& point, double coefficient)
{
  if (coefficient <= 0.0)
  {
    throw std::invalid_argument("Scaling coefficient must be positive");
  }

  for (size_t i = 0; i < count; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();
    double dx = frame.pos.x - point.x;
    double dy = frame.pos.y - point.y;

    shapes[i]->move(point);
    shapes[i]->unsafeScale(coefficient);

    double scaled_dx = -dx * coefficient;
    double scaled_dy = -dy * coefficient;
    shapes[i]->move(scaled_dx, scaled_dy);
  }
}

karpenko::rectangle_t karpenko::getOverallFrameRect(const Shape* const* shapes, size_t count)
{
  const rectangle_t emptyFrame = {0.0, 0.0, {0.0, 0.0}};
  if (count == 0)
  {
    return emptyFrame;
  }

  rectangle_t firstFrame = shapes[0]->getFrameRect();
  double halfWidth = firstFrame.width / 2.0;
  double halfHeight = firstFrame.height / 2.0;

  double left = firstFrame.pos.x - halfWidth;
  double right = firstFrame.pos.x + halfWidth;
  double bottom = firstFrame.pos.y - halfHeight;
  double top = firstFrame.pos.y + halfHeight;

  for (size_t i = 1; i < count; ++i)
  {
    rectangle_t frame = shapes[i]->getFrameRect();
    double currentHalfWidth = frame.width / 2.0;
    double currentHalfHeight = frame.height / 2.0;

    double currentLeft = frame.pos.x - currentHalfWidth;
    double currentRight = frame.pos.x + currentHalfWidth;
    double currentBottom = frame.pos.y - currentHalfHeight;
    double currentTop = frame.pos.y + currentHalfHeight;

    left = std::min(left, currentLeft);
    right = std::max(right, currentRight);
    bottom = std::min(bottom, currentBottom);
    top = std::max(top, currentTop);
  }

  double width = right - left;
  double height = top - bottom;
  double centerX = (left + right) / 2.0;
  double centerY = (bottom + top) / 2.0;

  return {width, height, {centerX, centerY}};
}

void karpenko::printFrameRectInfo(const rectangle_t& frame, const char* prefix)
{
  std::cout << prefix << "Frame Rect:\n";
  std::cout << prefix << "  Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
  std::cout << prefix << "  Width: " << frame.width << "\n";
  std::cout << prefix << "  Height: " << frame.height << "\n";
}

void karpenko::printAllInfo(const Shape* const* shapes, size_t count, const char* title)
{
  std::cout << "\n" << title << ":\n";

  double totalArea = 0.0;
  for (size_t i = 0; i < count; ++i)
  {
    std::cout << "Shape " << i + 1 << ":\n";
    double area = shapes[i]->getArea();
    std::cout << "  Area: " << std::fixed << std::setprecision(2) << area << "\n";
    totalArea += area;
  }

  std::cout << "\nTotal area: " << std::fixed << std::setprecision(2) << totalArea << "\n";

  rectangle_t overallFrame = getOverallFrameRect(shapes, count);
  printFrameRectInfo(overallFrame, "");
}
