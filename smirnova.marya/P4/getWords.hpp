#ifndef GETWORDS_HPP
#define GETWORDS_HPP
#include <cstdlib>
#include <istream>

namespace smirnova
{
  char** getWords(std::istream& in, size_t& size,
  bool (*checkSpace)(char));
}

#endif
