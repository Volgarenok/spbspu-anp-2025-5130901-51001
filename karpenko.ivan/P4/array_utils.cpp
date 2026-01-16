#include "array_utils.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>

namespace karpenko
{
  bool is_word_char(char ch)
  {
    return !std::isspace(static_cast< unsigned char >(ch));
  }

  char** read_words(std::istream& in, size_t& word_count)
  {
    const size_t MAX_WORDS = 100;
    const size_t INITIAL_CAPACITY = 16;
    const double GROW_FACTOR = 1.5;

    char** words = new char*[MAX_WORDS];

    word_count = 0;
    char c;
    bool in_word = false;
    size_t word_size = 0;
    char* current_word = nullptr;

    auto finish_word = [&]()
    {
      if (in_word && word_size > 0)
      {
        if (word_count >= MAX_WORDS)
        {
          for (size_t i = 0; i < word_count; ++i)
          {
            delete[] words[i];
          }
          delete[] words;
          std::cerr << "Error: too many words\n";
          std::exit(1);
        }
        words[word_count] = current_word;
        word_count++;
        in_word = false;
        word_size = 0;
        current_word = nullptr;
      }
    };

    while (in.get(c))
    {
      if (c == '\n')
      {
        finish_word();
        break;
      }

      if (is_word_char(c))
      {
        if (!in_word)
        {
          in_word = true;
          current_word = new char[INITIAL_CAPACITY];
          word_size = 0;
        }

        if (word_size >= INITIAL_CAPACITY - 1)
        {
          size_t new_capacity = static_cast< size_t >(INITIAL_CAPACITY * GROW_FACTOR);
          char* new_buffer = nullptr;
          try
          {
            new_buffer = new char[new_capacity];
          }
          catch (const std::bad_alloc&)
          {
            delete[] current_word;
            throw;
          }

          std::memcpy(new_buffer, current_word, word_size);
          delete[] current_word;
          current_word = new_buffer;
        }

        current_word[word_size++] = c;
        current_word[word_size] = '\0';
      }
      else
      {
        finish_word();
      }
    }

    finish_word();

    if (word_count == 0)
    {
      delete[] words;
      return nullptr;
    }

    char** resized_words = new char*[word_count];
    for (size_t i = 0; i < word_count; ++i)
    {
      resized_words[i] = words[i];
    }
    delete[] words;

    return resized_words;
  }

  void resize_char_array(char*& array, size_t& capacity,
    size_t required_size)
  {
    const double GROW_FACTOR = 1.5;
    size_t new_capacity = static_cast< size_t >(capacity * GROW_FACTOR);
    
    if (new_capacity <= capacity)
    {
      new_capacity = capacity + 1;
    }
    
    while (new_capacity < required_size)
    {
      new_capacity = static_cast< size_t >(new_capacity * GROW_FACTOR);
    }

    char* new_array = nullptr;
    try
    {
      new_array = new char[new_capacity];
    }
    catch (const std::bad_alloc&)
    {
      delete[] array;
      throw;
    }

    std::memcpy(new_array, array, capacity);
    delete[] array;
    array = new_array;
    capacity = new_capacity;
  }
}
