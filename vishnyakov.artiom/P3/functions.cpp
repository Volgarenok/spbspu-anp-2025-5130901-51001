#include "functions.hpp"

size_t vishnyakov::move(size_t type, size_t column, size_t start)
{
  if (start == 0)
  {
    return 1;
  }
  switch (type % 4)
  {
  case 0:
    return start - column;
    break;
  case 1:
    return start + 1;
    break;
  case 2:
    return start + column;
    break;
  case 3:
    return start - 1;
    break;
  }
  return start;
}
bool vishnyakov::isIn(const size_t *array, size_t value, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    if (value == array[i])
    {
      return true;
    }
  }
  return false;
}
