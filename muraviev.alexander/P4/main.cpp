#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>

namespace muraviev {
  void copyEls(char* copy_to, const char* copy_from, size_t len);
  char* getline(std::istream& in, size_t& size);
  char* intertwinedLine(char* tmp, const char* line1, const char* line2, 
  size_t size_1, size_t size_2);
  char* digitsAddedLine(char* tmp, const char* add_to, const char* take_from, 
  size_t size_1, size_t size_2);
}

void muraviev::copyEls(char* copy_to, const char* copy_from, size_t len)
{
  for (size_t i = 0; i < len; ++i) {
    copy_to[i] = copy_from[i];
  }
}

char* muraviev::getline(std::istream& in, size_t& size)
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

char* muraviev::intertwinedLine(char* tmp, const char* line1, const char* line2, 
  size_t size_1, size_t size_2)
{
  size_t bigSize = size_1 + size_2;
  size_t left = 0, right = 0;
  
  for (size_t i = 0; i < bigSize; ++i) {
    bool takeLeft = true;

    if (left >= size_1) {
      takeLeft = false;
    } else if (right >= size_2) {
      takeLeft = true;
    } else {
      takeLeft = (i % 2 == 0);
    }

    if (takeLeft) {
      tmp[i] = line1[left++];
    } else {
      tmp[i] = line2[right++];
    }
  }

  tmp[bigSize] = 0;
  return tmp;
}

char* muraviev::digitsAddedLine(char* tmp, const char* add_to, const char* take_from, 
  size_t size_1, size_t size_2)
{
  copyEls(tmp, add_to, size_1);

  size_t resIndex = size_1;
  for (size_t i = 0; i < size_2; ++i) {
    if (std::isdigit(take_from[i])) {
      tmp[resIndex++] = take_from[i];
    }
  }

  tmp[resIndex] = 0;
  return tmp;
}

int main()
{
  size_t size_1 = 0;
  char* line1 = nullptr;

  try {
    line1 = muraviev::getline(std::cin, size_1);
  } catch (const std::bad_alloc& e) {
    std::cerr << "Allocation error: " << e.what() << '\n';
    return 1;
  } catch (const std::logic_error& e) {
    std::cerr << "Logic error: " << e.what() << '\n';
    return 1;
  }

  const char* line2 = "def ";
  size_t size_2 = std::strlen(line2);

  char* inter_tmp = (char*)malloc(sizeof(char) * (size_1 + size_2 + 1));

  if (!inter_tmp) {
    std::cerr << "Allocation error\n";
    free(line1);
    return 1;
  }

  char* intertwined = muraviev::intertwinedLine(inter_tmp, line1, line2, size_1, size_2);
  
  std::cout << intertwined << "\n";

  const char* line3 = "g1h2k";
  size_t size_3 = std::strlen(line3);

  size_t digsFound = 0;
  for (size_t i = 0; i < size_3; ++i) {
    if (std::isdigit(line3[i])) {
      digsFound++;
    }
  }

  char* withDigs_tmp = (char*)malloc(sizeof(char) * (size_1 + digsFound + 1));

  if (!withDigs_tmp) {
    std::cerr << "Allocation error\n";
    free(line1);
    free(intertwined);
    return 1;
  }

  char* lineWithDigs = muraviev::digitsAddedLine(withDigs_tmp, line1, line3, size_1, size_3);

  std::cout << lineWithDigs << "\n";
  
  free(line1);
  free(intertwined);
  free(lineWithDigs);
  return 0;
}
