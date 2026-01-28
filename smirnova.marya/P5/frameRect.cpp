#include "frameRect.hpp"
#include <algorithm>

smirnova::rectangle_t smirnova::getTotalFrameRect(const Shape* const shapes[], size_t count) noexcept
{
  if (count == 0)
  {
    return {0.0, 0.0, {0.0, 0.0}};
  }
  rectangle_t total = shapes[0]->getFrameRect();
  for (size_t i = 1; i < count; ++i)
  {
    rectangle_t f = shapes[i]->getFrameRect();
    double minX = std::min(total.pos.x - total.width / 2, f.pos.x - f.width / 2);
    double maxX = std::max(total.pos.x + total.width / 2, f.pos.x + f.width / 2);
    double minY = std::min(total.pos.y - total.height / 2, f.pos.y - f.height / 2);
    double maxY = std::max(total.pos.y + total.height / 2, f.pos.y + f.height / 2);
    total.width = maxX - minX;
    total.height = maxY - minY;
    total.pos = {(minX + maxX) / 2.0, (minY + maxY) / 2.0};
  }
  return total;
}


