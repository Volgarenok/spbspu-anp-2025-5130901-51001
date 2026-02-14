#include <iostream>
#include <cstring>
#include <cctype>
#include <iomanip>

namespace nabieva
{
	int formAllLetterString(char* input, char** result)
	{
		unsigned char letters[26] = { 0 };
		for (size_t i = 0; i < std::strlen(input); ++i) {
			unsigned char c = static_cast<unsigned char> (input[i]);
			if (std::isalpha(c)) {
				c = std::tolower(c);
				if (c >= 'a' && c <= 'z') {
					letters[c - 'a'] = 1;
				}
			}
		}
		const char* secondString = "def ghk";
		for (size_t i = 0; i < std::strlen(secondString); ++i) {
			unsigned char c = static_cast<unsigned char> (secondString[i]);
			letters[c - 'a'] = 1;
		}
		size_t pos = 0;
		for (int i = 0; i < 26; ++i) {
			if (letters[i]) {
				pos++;
			}
		}
		*result = new char[pos + 1];
		pos = 0;
		for (int i = 0; i < 26; ++i) {
			if (letters[i]) {
				(*result)[pos++] = static_cast<char> ('a' + i);;
			}
		}
		(*result)[pos] = '\0';
		return pos;
	}

	int mergeString(char* input, char** result)
	{
		const char* secondString = "def ";
		size_t pos = 0;
		*result = new char[std::max(std::strlen(input), std::strlen(secondString)) + 1];
		for (size_t i = 0; i < std::strlen(input) || i < std::strlen(secondString); i++) {
			if (i < std::strlen(input)) {
				(*result)[pos++] = input[i];
			}
			if (i < std::strlen(secondString)) {
				(*result)[pos++] = secondString[i];
			}
		}
		*result[pos] = '\0';
		return pos;
	}

	char* getline(std::istream& in, size_t& length)
	{
		bool is_skipws = in.flags() & std::ios_base::skipws;
		if (is_skipws) {
			in >> std::noskipws;
		}
		length = 0;
		size_t capacity = 16;
		char* buffer = new char[capacity];
		if (!buffer) {
			return nullptr;
		}
		while (in >> buffer[length]) {
			if (buffer[length] == '\n') {
				break;
			}
			if (length + 1 >= capacity) {
				size_t newCapacity = capacity * 2;
				char* newBuffer = new char[newCapacity];
				if (!newBuffer) {
					delete[] buffer;
					return nullptr;
				}
				for (size_t i = 0; i < length; i++) {
					newBuffer[i] = buffer[i];
				}
				delete[] buffer;
				buffer = newBuffer;
				capacity = newCapacity;
			}
			length++;
		}
		if (in.fail()) {
			delete[] buffer;
			return nullptr;
		}
		buffer[length] = '\0';
		if (is_skipws) {
			in >> std::skipws;
		}
		return buffer;
	}
}

int main()
{
	size_t inputSize = 0;
	char* input = nabieva::getline(std::cin, inputSize);
	char* result = nullptr;
	if (!input) {
		std::cerr << "can't give input memory\n";
		return 1;
	}
	size_t resultSize = nabieva::formAllLetterString(input, &result);
	for (size_t i = 0; i < resultSize; i++) {
		std::cout << result[i];
	}
	std::cout << "\n";
	resultSize = nabieva::mergeString(input, &result);
	for (size_t i = 0; i < resultSize; i++) {
		std::cout << result[i];
	}
	delete[] input;
	delete[] result;
	return 0;
}