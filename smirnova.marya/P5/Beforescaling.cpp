#include "Beforescaling.hpp"
#include <iostream>
#include <stdexcept>

#include "frameRect.hpp"
#include "shape.hpp"

namespace smirnova
{
  void initialData(Shape *shapes[], const char *names[3])
  {
    double totalArea = 0.0;
    for (int i = 0; i < 3; ++i)
    {
      double area = shapes[i]->getArea();
      rectangle_t frame = shapes[i]->getFrameRect();
      totalArea += area;
      std::cout << names[i] << ": square = " << area << "\n";
      std::cout << "Bounding rectangle: width = " << frame.width << ", ";
      std::cout << "height = " << frame.height << ", ";
      std::cout << "center = (" << frame.pos.x << ", " << frame.pos.y << ")\n";
    }
    std::cout << "Total area: " << totalArea << "\n";
    rectangle_t totalFrame = getTotalFrameRect(shapes, 3);
    std::cout << "Overall bounding box: width = " << totalFrame.width << ", ";
    std::cout << "height = " << totalFrame.height << ", ";
    std::cout << "center = (" << totalFrame.pos.x << ", " << totalFrame.pos.y << ")\n";
  }
}

