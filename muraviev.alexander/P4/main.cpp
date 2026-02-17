#include <iostream>
#include <iomanip>
#include <cctype>
#include <cstring>

namespace muraviev {
  void copyEls(char* copy_to, const char* copy_from, size_t size);
  char* getline(std::istream& in, size_t& foundSize);
  char* intertwinedLine(char* tmp, const char* line1, const char* line2,
    size_t size_1, size_t size_2);
  char* digitsAddedLine(char* result, const char* add_to, const char* take_from,
    size_t size_1);
}

void muraviev::copyEls(char* copy_to, const char* copy_from, size_t size)
{
  for (size_t i = 0; i < size; ++i) {
    copy_to[i] = copy_from[i];
  }
}

char* muraviev::getline(std::istream& in, size_t& foundSize)
{
  bool is_skips = in.flags() & std::ios_base::skipws;
  size_t capacity = 16;
  size_t size = 0;
  char* line = reinterpret_cast< char* >(malloc(sizeof(char) * capacity));

  if (!line) {
    foundSize = 0;
    return nullptr;
  }

  if (is_skips) {
    in >> std::noskipws;
  }

  char sym;
  while (!(in >> sym).fail() && sym != '\n') {
    if (size + 1 >= capacity) {
      size_t new_capacity = capacity * 2;
      char* tmp_line = reinterpret_cast< char* >(malloc(sizeof(char) * new_capacity));

      if (!tmp_line) {
        free(line);
        foundSize = 0;
        if (is_skips) {
          in >> std::skipws;
        }
        return nullptr;
      }

      copyEls(tmp_line, line, size);
      free(line);
      capacity = new_capacity;
      line = tmp_line;
    }

    line[size++] = sym;
  }

  if (is_skips) {
    in >> std::skipws;
  }

  if (size == 0 && !in) {
    free(line);
    foundSize = 0;
    return nullptr;
  }

  line[size] = '\0';
  foundSize = size;
  return line;
}

char* muraviev::intertwinedLine(char* tmp, const char* line1, const char* line2,
  size_t size_1, size_t size_2)
{
  size_t bigSize = size_1 + size_2;
  size_t left = 0;
  size_t right = 0;

  for (size_t i = 0; i < bigSize; ++i) {
    bool takeLeft = false;

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

  tmp[bigSize] = '\0';
  return tmp;
}

char* muraviev::digitsAddedLine(char* result, const char* add_to, const char* take_from,
  size_t size_1)
{
  copyEls(result, add_to, size_1);
  size_t resIndex = size_1;

  for (size_t i = 0; take_from[i] != '\0'; ++i) {
    if (std::isdigit(take_from[i])) {
      result[resIndex++] = take_from[i];
    }
  }

  result[resIndex] = '\0';
  return result;
}

int main()
{
  size_t size_1 = 0;
  char* line1 = muraviev::getline(std::cin, size_1);

  if (line1 == nullptr) {
    std::cerr << "Error when getting a line." << '\n';
    return 1;
  }

  const char* line2 = "def ";

  size_t size_2 = std::strlen(line2);
  size_t inter_size = size_1 + size_2;

  char* inter_tmp = reinterpret_cast< char* >(malloc(sizeof(char) * (inter_size + 1)));

  if (!inter_tmp) {
    std::cerr << "Allocation error\n";
    free(line1);
    return 1;
  }
  inter_tmp[inter_size] = '\0';

  char* intertwined = muraviev::intertwinedLine(inter_tmp, line1, line2, size_1, size_2);
  std::cout << intertwined << "\n";

  const char* line3 = "g1h2k";

  size_t size_3 = std::strlen(line3);
  size_t withDigs_size = size_1 + size_3;

  char* withDigs_tmp = reinterpret_cast< char* >(malloc(sizeof(char) * (withDigs_size + 1)));

  if (!withDigs_tmp) {
    std::cerr << "Allocation error\n";
    free(line1);
    free(intertwined);
    return 1;
  }
  withDigs_tmp[withDigs_size] = '\0';

  char* lineWithDigs = muraviev::digitsAddedLine(withDigs_tmp, line1, line3, size_1);
  std::cout << lineWithDigs << "\n";

  free(line1);
  free(intertwined);
  free(lineWithDigs);
  return 0;
}
