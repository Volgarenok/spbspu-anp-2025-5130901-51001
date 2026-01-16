#ifndef STRING_READER_HPP
#define STRING_READER_HPP

#include <iostream>
#include <cstddef>

namespace karpenko
{
  char* read_string_with_amortization(std::istream& in, size_t& size,
    bool (*check_char)(char));
}

#endif
