#ifndef FRAMERECT_HPP
#define FRAMERECT_HPP
#include <cstddef>

#include "shape.hpp"

namespace smirnova
{
  rectangle_t getTotalFrameRect(const Shape *shapes[], const size_t count) noexcept;
}

#endif
