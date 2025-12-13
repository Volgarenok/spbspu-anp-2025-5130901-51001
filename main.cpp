#include <iostream>
#include <iomanip>

namespace muraviev
{
  void copyEls(char* copy_to, const char* copy_from, size_t len);
  char * getline(std::istream & in, size_t & size);
}

void muraviev::copyEls(char* copy_to, const char* copy_from, size_t len)
{
  for (size_t i = 0; i < len; ++i) {
    copy_to[i] = copy_from[i];
  }
}

char * muraviev::getline(std::istream & in, size_t & size)
{
  bool is_skips = in.flags() & std::ios_base::skipws;

  size_t buffer = 16;
  size_t len = 0;
  char* line = (char*)malloc(sizeof(char) * buffer);
  
  if (!line) {
    size = 0;
    throw std::bad_alloc();
  }

  if (is_skips) {
    in >> std::noskipws;
  }

  char sym;
  while (!(in >> sym).fail()) {
    if (sym == '\n') {
      break;
    }

    if (len + 1 >= buffer) {
      size_t new_buffer = buffer * 2;
      char* tmp_line = (char*)malloc(sizeof(char) * new_buffer);

      if (!tmp_line) {
        free(line);
        size = 0;

        if (is_skips) {
          in >> std::skipws;
        }
        throw std::bad_alloc();
      }

      copyEls(tmp_line, line, len);
      free(line);

      buffer = new_buffer;
      line = tmp_line;
    }

    line[len] = sym;
    len++;
  }

  if (is_skips) {
    in >> std::skipws;
  }

  if (len == 0 && !in) {
    free(line);
    size = 0;
    throw std::logic_error("Input failed");
  }

  line[len] = 0;
  size = len;
  return line;
}


int main()
{
  size_t lineSize = 0;
  char * line = nullptr;

  try {
    line = muraviev::getline(std::cin, lineSize);
  } catch (const std::bad_alloc& e) {
    std::cerr << "Allocation error: " << e.what() << '\n';
    return 1;
  } catch (const std::logic_error& e) {
    std::cerr << "Logic error: " << e.what() << '\n';
    return 1;
  }
  
  std::cout << line;
  free(line);
}
