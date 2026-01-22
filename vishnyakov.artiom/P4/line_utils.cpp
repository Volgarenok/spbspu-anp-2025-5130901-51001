#include "line_utils.hpp"

bool vishnyakov::isIn(char *str, char letter)
{
  for (size_t i = 0; str[i] != '\0'; ++i)
  {
    if (str[i] == letter)
    {
      return 1;
    }
  }
  return 0;
}

void vishnyakov::copy(const char *a, char *b, size_t l)
{
  for (size_t i = 0; i < l; ++i)
  {
    b[i] = a[i];
  }
}

bool vishnyakov::isSpace(char letter)
{
  return std::isspace(static_cast< unsigned char >(letter));
}

