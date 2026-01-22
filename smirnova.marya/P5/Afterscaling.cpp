#include "Afterscaling.hpp"
#include <iostream>

#include "frameRect.hpp"
#include "shape.hpp"

namespace smirnova
{
  void getResult(Shape *shapes[], const char *names[3])
  {
    double totalArea = 0.0;
    for (size_t i = 0; i < 3; ++i)
    {
      double area = shapes[i]->getArea();
      rectangle_t frame = shapes[i]->getFrameRect();
      totalArea += area;
      std::cout << names[i] << ": area = " << area << "\n";
      std::cout << "Bounding rectangle: width = " << frame.width << ", height = " << frame.height
                << ", center = (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    }
    std::cout << "Total area: " << totalArea << "\n";
    rectangle_t totalFrame = getTotalFrameRect(shapes, 3);
    std::cout << "Overall bounding box: width = " << totalFrame.width
              << ", height = " << totalFrame.height << ", center = (" << totalFrame.pos.x << ", "
              << totalFrame.pos.y << ")\n";
  }
} // namespace smirnova
