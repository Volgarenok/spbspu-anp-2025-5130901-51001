#include "inputData.hpp"
#include <iostream>
#include <cstddef>

#include "frameRect.hpp"
#include "shape.hpp"

void smirnova::inputData(const Shape* const shapes[], const char* const names[], size_t sizeShapes)
{
  double totalArea = 0.0;
  for (size_t i = 0; i < sizeShapes; ++i)
  {
    double area = shapes[i]->getArea();
    rectangle_t frame = shapes[i]->getFrameRect();
    totalArea += area;
    std::cout << names[i] << ": area = " << area << "\n";
    std::cout << "Bounding rectangle: width = " << frame.width << ", ";
    std::cout << "height = " << frame.height << ", ";
    std::cout << "center = (" << frame.pos.x << ", " << frame.pos.y << ")\n";
  }
  std::cout << "Total area: " << totalArea << "\n";
  rectangle_t totalFrame = getTotalFrameRect(shapes, sizeShapes);
  std::cout << "Overall bounding box: width = " << totalFrame.width << ", ";
  std::cout << "height = " << totalFrame.height << ", ";
  std::cout << "center = (" << totalFrame.pos.x << ", " << totalFrame.pos.y << ")\n";
}

