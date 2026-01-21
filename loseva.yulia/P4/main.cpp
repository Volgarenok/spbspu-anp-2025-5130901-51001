#include <iostream>
#include <istream>
#include <cctype>
#include <new>
namespace loseva
{
  char* mergeStrings(const char* first, const char* second, size_t first_len, size_t second_len);
  char* readInputLine(std::istream& input_stream, size_t& total_length);
  void extractLatins(const char* str1, const char* str2, char* output_buffer, size_t buffer_capacity);
  void removeLatins(const char* source, char* output_buffer, size_t buffer_capacity);
  struct ArrayGuard {
    char* data;
    ArrayGuard(char* p) : data(p) {}
    ~ArrayGuard() { delete[] data; }
  };
}
char* loseva::mergeStrings(const char* first, const char* second, size_t first_len, size_t second_len)
{
  char* combined = new char[first_len + second_len];
  if (first) {
    for (size_t i = 0; i < first_len; ++i) combined[i] = first[i];
  }
  if (second) {
    for (size_t i = 0; i < second_len; ++i) combined[first_len + i] = second[i];
  }
  return combined;
}
char* loseva::readInputLine(std::istream& input_stream, size_t& total_length)
{
  const size_t chunk_size = 10;
  char* full_string = nullptr;
  total_length = 0;
  char* current_chunk = new char[chunk_size];
  ArrayGuard chunk_guard(current_chunk);
  input_stream >> std::noskipws;
  char current_char;
  size_t chunk_index = 0;
  while ((input_stream >> current_char) && current_char != '\n') {
    if (chunk_index == chunk_size) {
      char* temp = mergeStrings(full_string, current_chunk, total_length, chunk_size);
      delete[] full_string;
      full_string = temp;
      total_length += chunk_size;
      chunk_index = 0;
    }
    current_chunk[chunk_index++] = current_char;
  }
  char* final_string = new char[total_length + chunk_index + 1];
  if (full_string) {
    for (size_t i = 0; i < total_length; ++i) final_string[i] = full_string[i];
  }
  for (size_t i = 0; i < chunk_index; ++i) final_string[total_length + i] = current_chunk[i];
  total_length += chunk_index;
  final_string[total_length] = '\0';
  delete[] full_string;
  return final_string;
}
void loseva::extractLatins(const char* str1, const char* str2, char* output_buffer, size_t buffer_capacity)
{
  bool latin_present[26] = {false};
  size_t write_pos = 0;
  auto mark_latins = [&](const char* s) {
    for (size_t i = 0; s[i] != '\0'; ++i) {
      if (std::isalpha(static_cast<unsigned char>(s[i]))) {
        latin_present[std::tolower(static_cast<unsigned char>(s[i])) - 'a'] = true;
      }
    }
  };
  mark_latins(str1);
  mark_latins(str2);
  for (int i = 0; i < 26; ++i) {
    if (latin_present[i]) {
      if (write_pos + 1 >= buffer_capacity) throw std::bad_alloc();
      output_buffer[write_pos++] = static_cast<char>('a' + i);
    }
  }
  output_buffer[write_pos] = '\0';
}
void loseva::removeLatins(const char* source, char* output_buffer, size_t buffer_capacity)
{
  size_t write_pos = 0;
  for (size_t i = 0; source[i] != '\0'; ++i) {
    if (!std::isalpha(static_cast<unsigned char>(source[i]))) {
      if (write_pos + 1 >= buffer_capacity) throw std::bad_alloc();
      output_buffer[write_pos++] = source[i];
    }
  }
  output_buffer[write_pos] = '\0';
}
int main()
{
  char* user_input = nullptr;
  char* non_latin_string = nullptr;
  try {
    size_t input_length = 0;
    user_input = loseva::readInputLine(std::cin, input_length);
    char unique_latins[27];
    non_latin_string = new char[input_length + 1];
    loseva::extractLatins(user_input, "defghk", unique_latins, sizeof(unique_latins));
    std::cout << "Unique latins: " << unique_latins << "\n";
    loseva::removeLatins(user_input, non_latin_string, input_length + 1);
    std::cout << "Filtered string: " << non_latin_string << "\n";
    delete[] user_input;
    delete[] non_latin_string;
  }
  catch (const std::bad_alloc&) {
    delete[] user_input;
    delete[] non_latin_string;
    std::cerr << "Error: Memory limit exceeded or buffer overflow.\n";
    return 1;
  }
  return 0;
}
