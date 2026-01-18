#ifndef STRING_READER_HPP
#define STRING_READER_HPP

#include <iostream>
#include <cstddef>

namespace karpenko
{
  char* readStringWithAmortization(std::istream& in, size_t& size,
    bool (*checkChar)(char));
}

#endif
