#include <iostream>
#include <iomanip>

namespace muraviev
{
  void copyEls(char* copy_to, const char* copy_from, size_t len);
  char * getline(std::istream & in, size_t & size);
  char * intertwineStrings(const char* first, const char* second, size_t firstSize, size_t secondSize);
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

char * muraviev::intertwineStrings(const char * first, const char * second, size_t firstSize, size_t secondSize)
{
  size_t bigSize = firstSize + secondSize;
  char * intertwined = (char*)malloc(sizeof(char) * (bigSize) + 1);

  if (!intertwined) {
    throw std::bad_alloc();
  }

  size_t left = 0, right = 0;
  
  for (size_t i = 0; i < bigSize; ++i) {
    bool takeLeft = true;

    if (left >= firstSize) {
      takeLeft = false;
    } else if (right >= secondSize) {
      takeLeft = true;
    } else {
      takeLeft = (i % 2 == 0);
    }

    if (takeLeft) {
      intertwined[i] = first[left++];
    } else {
      intertwined[i] = second[right++];
    }
  }

  intertwined[bigSize] = 0;
  return intertwined;
}


int main()
{
  size_t lineSize = 0, lineSize2 = 0;
  char * line = nullptr;
  char * line2 = nullptr;

  try {
    line = muraviev::getline(std::cin, lineSize);
    line2 = muraviev::getline(std::cin, lineSize2);
  } catch (const std::bad_alloc& e) {
    std::cerr << "Allocation error: " << e.what() << '\n';
    return 1;
  } catch (const std::logic_error& e) {
    std::cerr << "Logic error: " << e.what() << '\n';
    return 1;
  }

  free(line);
  free(line2);

  line = muraviev::intertwineStrings(line, line2, lineSize, lineSize2);
  
  std::cout << "Result: " << line << "\n";
}
