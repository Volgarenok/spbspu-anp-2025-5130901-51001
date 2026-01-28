#ifndef GETLINE_HPP
#define GETLINE_HPP
#include <fstream>
#include "line_utils.hpp"

namespace vishnyakov
{
  char **getWords(std::istream &input, size_t &size, bool (*isSpace)(char));
}

#endif

