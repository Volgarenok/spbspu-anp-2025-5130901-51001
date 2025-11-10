#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace alekseev
{
    const size_t MAX_FIXED_SIZE = 10000;

    bool readMatrix(std::ifstream& input, int* matrix, size_t rows, size_t cols)
    {
        for (size_t i = 0; i < rows * cols; ++i) {
            if (!(input >> matrix[i])) {
                return false;
            }
        }
        return true;
    }

    int cSP(const int* matrix, size_t rows, size_t cols)
    {
        if (rows == 0 || cols == 0) {
            return 0;
        }

        int sC = 0;

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                int current = matrix[i * cols + j];
                bool isMinInRow = true;
                bool isMaxInCol = true;

                for (size_t k = 0; k < cols; ++k) {
                    if (matrix[i * cols + k] < current) {
                        isMinInRow = false;
                        break;
                    }
                }

                if (!isMinInRow) {
                    continue;
                }

                for (size_t k = 0; k < rows; ++k) {
                    if (matrix[k * cols + j] > current) {
                        isMaxInCol = false;
                        break;
                    }
                }

                if (isMaxInCol) {
                    ++sC;
                }
            }
        }

        return sC;
    }

    size_t sectask(const int* matrix, size_t rows, size_t cols)
    {
        if (rows == 0 || cols == 0) {
            return 0;
        }

        size_t maxColumn = 0;
        size_t maxLength = 0;

        for (size_t j = 0; j < cols; ++j) {
            size_t currentLength = 1;
            size_t columnMaxLength = 1;

            for (size_t i = 1; i < rows; ++i) {
                if (matrix[i * cols + j] == matrix[(i - 1) * cols + j]) {
                    ++currentLength;
                }
                else {
                    if (currentLength > columnMaxLength) {
                        columnMaxLength = currentLength;
                    }
                    currentLength = 1;
                }
            }

            if (currentLength > columnMaxLength) {
                columnMaxLength = currentLength;
            }

            if (columnMaxLength > maxLength) {
                maxLength = columnMaxLength;
                maxColumn = j;
            }
        }

        return maxColumn + 1;
    }

    void writeResults(std::ofstream& output, int saddleCount, size_t seriesColumn)
    {
        output << saddleCount << "\n";
        output << seriesColumn << "\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << "\n";
        return 1;
    }

    std::string numStr = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    int taskNum = 0;
    try {
        taskNum = std::stoi(numStr);
    }
    catch (const std::exception&) {
        std::cerr << "First parameter is not a number\n";
        return 1;
    }

    if (taskNum != 1 && taskNum != 2) {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }

    std::ifstream input(inputFile);
    if (!input.is_open()) {
        std::cerr << "Cannot open input file\n";
        return 2;
    }

    size_t rows = 0;
    size_t cols = 0;
    if (!(input >> rows >> cols)) {
        std::cerr << "Cannot read matrix dimensions\n";
        return 2;
    }

    if (rows == 0 && cols == 0) {
        std::ofstream output(outputFile);
        if (!output.is_open()) {
            std::cerr << "Cannot open output file\n";
            return 2;
        }
        alekseev::writeResults(output, 0, 0);
        return 0;
    }

    if (rows * cols > alekseev::MAX_FIXED_SIZE && taskNum == 1) {
        std::cerr << "Matrix size exceeds fixed array capacity\n";
        return 2;
    }

    int* matrix = nullptr;
    if (taskNum == 1) {
        matrix = new int[alekseev::MAX_FIXED_SIZE];
    }
    else {
        matrix = new int[rows * cols];
    }

    bool readSuccess = alekseev::readMatrix(input, matrix, rows, cols);
    input.close();

    if (!readSuccess) {
        std::cerr << "Cannot read matrix elements\n";
        delete[] matrix;
        return 2;
    }

    int saddleCount = alekseev::cSP(matrix, rows, cols);
    size_t seriesColumn = alekseev::sectask(matrix, rows, cols);

    std::ofstream output(outputFile);
    if (!output.is_open()) {
        std::cerr << "Cannot open output file\n";
        delete[] matrix;
        return 2;
    }

    alekseev::writeResults(output, saddleCount, seriesColumn);
    output.close();

    delete[] matrix;
    return 0;
}
