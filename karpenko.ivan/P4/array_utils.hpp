#ifndef ARRAY_UTILS_HPP
#define ARRAY_UTILS_HPP

#include <iostream>

namespace karpenko
{
  bool is_word_char(char ch);

  char** read_words(std::istream& in, size_t& word_count);

  void resize_char_array(char*& array, size_t& capacity, size_t required_size);
}

#endif