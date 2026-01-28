#ifndef GETLINE_HPP
#define GETLINE_HPP
#include <cstdlib>
#include <istream>

namespace smirnova
{
  char* getLine(std::istream& in, size_t& size);
}

#endif
