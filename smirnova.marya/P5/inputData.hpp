#ifndef INPUTDATA_HPP
#define INPUTDATA_HPP
#include <cstddef>

#include "shape.hpp"

namespace smirnova
{
  void inputData(const Shape* const shapes[], const char* const names[], size_t sizeShapes);
}

#endif

