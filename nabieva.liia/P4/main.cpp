#include <iostream>
#include <cstring>

int main()
{
	size_t MAX_INPUT_SIZE = 1024;
	size_t MAX_RESULT_SIZE = 2048;
	char* input = new char[MAX_INPUT_SIZE];
	if (!input) {
		std::cerr << "can't give input memory" << std::endl;
		return 1;
	}
	char* result = new char[MAX_RESULT_SIZE];
	if (!result) {
		std::cerr << "can't give result memory" << std::endl;
		delete[] input;
		return 1;
	}
	if (!std::cin.getline(input, MAX_INPUT_SIZE)) {
		if (std::cin.fail()) {
			std::cerr << "input error or too long string" << std::endl;
			delete[] input;
			delete[] result;
			return 1;
		}
	}
	for (size_t i = 0; i < strlen(input); i++) {
		std::cout << input[i];
	}
	std::cout << "\n";


	delete[] input;
	delete[] result;
	return 0;
}
