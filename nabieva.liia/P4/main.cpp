#include <iostream>
#include <cstring>
#include <cctype>

namespace nabieva 
{
	int formAllLetterString(char* input, char* result, size_t maxResultSize)
	{
		unsigned char letters[26] = { 0 };
		for (size_t i = 0; i < strlen(input); ++i) {
			unsigned char c = static_cast<unsigned char>(input[i]);
			if (std::isalpha(c)) {
				c = std::tolower(c);
				if (c >= 'a' && c <= 'z') {
					letters[c - 'a'] = 1;
				}
			}
		}
		const char* secondString = "def ghk";
		for (size_t i = 0; i < strlen(secondString); ++i) {
			unsigned char c = static_cast<unsigned char>(secondString[i]);
			if (std::isalpha(c)) {
				c = std::tolower(c);
				if (c >= 'a' && c <= 'z') {
					letters[c - 'a'] = 1;
				}
			}
		}
		size_t pos = 0;
		for (int i = 0; i < 26; ++i) {
			if (letters[i]) {
				if (pos >= maxResultSize) {
					return -1;
				}
				result[pos++] = static_cast<char>('a' + i);
			}
		}
		return pos;
	}

	int mergeString(char* input, char* result, size_t maxResultSize)
	{
		const char* secondString = "def ";
		if (strlen(input) + strlen(secondString) > maxResultSize) {
			return -1;
		}
		size_t pos = 0;
		for (size_t i = 0; i < strlen(input) + strlen(secondString); ++i) {
			if (i < strlen(input)) {
				result[pos] = input[i];
				pos++;
			}
			if (i < strlen(secondString)) {
				result[pos] = secondString[i];
				pos++;
			}
		}
		return pos;
	}
}

int main()
{
	using namespace nabieva;
	size_t MAX_INPUT_SIZE = 1024;
	size_t MAX_RESULT_SIZE = 2048;
	char* input = new char[MAX_INPUT_SIZE];
	if (!input) {
		std::cerr << "can't give input memory\n";
		return 1;
	}
	char* result = new char[MAX_RESULT_SIZE];
	if (!result) {
		std::cerr << "can't give result memory\n";
		delete[] input;
		return 1;
	}
	if (!std::cin.getline(input, MAX_INPUT_SIZE)) {
		if (std::cin.fail()) {
			std::cerr << "input error or too long string\n";
			delete[] input;
			delete[] result;
			return 1;
		}
	}
	int resultSize = formAllLetterString(input, result, MAX_RESULT_SIZE);
	if (resultSize != -1) {
		for (size_t i = 0; i < static_cast<size_t>(resultSize); i++) {
			std::cout << result[i];
		}
	}
	else {
		std::cerr << "too long string\n";
		delete[] input;
		delete[] result;
		return 1;
	}
	std::cout << "\n";
	resultSize = mergeString(input, result, MAX_RESULT_SIZE);
	if (resultSize != -1) {
		for (size_t i = 0; i < static_cast<size_t>(resultSize); i++) {
			std::cout << result[i];
		}
	}
	else {
		std::cerr << "too long string\n";
		delete[] input;
		delete[] result;
		return 1;
	}
	delete[] input;
	delete[] result;
	return 0;
}
