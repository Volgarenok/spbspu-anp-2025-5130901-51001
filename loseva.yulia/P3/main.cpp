#include <iostream>
#include <fstream>

namespace loseva
{
    void elementsdiag(size_t rows, size_t cols, int matrix[10000], const char* outputFile);
    void localmax(size_t rows, size_t cols, int* matrix, const char* outputFile);
}

void loseva::elementsdiag(size_t rows, size_t cols, int matrix[10000], const char* outputFile)
{
    size_t maxsum = 0;
    int n = (rows < cols ? rows : cols);
    for (int sum = 0; sum <= 2 * (n - 1); ++sum) {
        if (sum == n - 1) continue;
        size_t diagsum = 0;
        for (int i = 0; i < n; ++i) {
            int j = sum - i;
            if ((j >= 0) && (j < n)) {
                diagsum += matrix[i * cols + j];
            }
        }
        if (diagsum > maxsum) {
            maxsum = diagsum;
        }
    }
    std::ofstream(outputFile) << maxsum;
}

void loseva::localmax(size_t rows, size_t cols, int* matrix, const char* outputFile)
{
    size_t count = 0;
    for (size_t i = 1; i < rows - 1; i++) {
        for (size_t j = 1; j < cols - 1; j++) {

            int val = matrix[i * cols + j];
            if (matrix[i * cols + j - 1] < val &&
                matrix[i * cols + j + 1] < val &&
                matrix[(i - 1) * cols + j] < val &&
                matrix[(i + 1) * cols + j] < val &&
                matrix[(i - 1) * cols + j - 1] < val &&
                matrix[(i + 1) * cols + j + 1] < val &&
                matrix[(i + 1) * cols + j - 1] < val &&
                matrix[(i - 1) * cols + j + 1] < val) {
                count++;
            }
        }
    }
    std::ofstream(outputFile) << count;
    delete[] matrix;
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Not enough arguments\n";
        return 1;
    }

    if (argc > 4) {
        std::cerr << "Too many arguments\n";
        return 1;
    }

    char* end;
    long num = std::strtol(argv[1], &end, 10);
    if (*end != '\0' ||  num < 1 ||  num > 2) {
        std::cerr << "First parameter is incorrect\n";
        return 1;
    }

    std::ifstream input(argv[2]);
    if (!input.is_open()) {
        std::cerr << "Cannot open input file\n";
        return 2;
    }

    size_t rows, cols;
    input >> rows >> cols;
    if (input.fail()) {
        std::cerr << "Cannot read matrix dimensions\n";
        return 2;
    }

    if (rows == 0 || cols == 0) {
        std::ofstream output(argv[3]);
        output << "0 0\n0\n";
        return 0;
    }

    if ((num == 1)) {
        if (rows * cols > 10000) {
            std::cerr << "Matrix too large for fixed array\n";
            return 2;
        }
        int matrix[10000];
        for (size_t i = 0; i < rows * cols; i++) {
            input >> matrix[i];
            if (input.fail()) {
                std::cerr << "Cannot read matrix element\n";
                return 2;
            }
        }
        loseva::elementsdiag(rows, cols, matrix, argv[3]);
    }
    else {
        int* matrix = new int[rows * cols];
        for (size_t i = 0; i < rows * cols; i++) {
            input >> matrix[i];
            if (input.fail()) {
                std::cerr << "Cannot read matrix element\n";
                delete[] matrix;
                return 2;
            }
        }
        loseva::localmax(rows, cols, matrix, argv[3]);
    }

    return 0;
}
