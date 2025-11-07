#include <iostream>
#include <fstream>

namespace shaykhraziev {

void lft_bot_cnt(int** matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) return;

    int minDim = (rows < cols) ? rows : cols;
    int layers = (minDim + 1) / 2;
    int increment = 1;

    for (int layer = 0; layer < layers; ++layer) {
        int startRow = rows - 1 - layer;
        int endRow = layer;
        int startCol = layer;
        int endCol = cols - 1 - layer;

        if (startRow < endRow || startCol > endCol) break;

        for (int i = startRow; i >= endRow && startCol <= endCol; --i) {
            matrix[i][startCol] += increment++;
        }

        for (int j = startCol + 1; j <= endCol && endRow <= startRow; ++j) {
            matrix[endRow][j] += increment++;
        }

        for (int i = endRow + 1; i <= startRow && startCol < endCol; ++i) {
            matrix[i][endCol] += increment++;
        }

        for (int j = endCol - 1; j > startCol && endRow < startRow; --j) {
            matrix[startRow][j] += increment++;
        }
    }
}

int min_sum_sdg(int** matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) return 0;

    int minSum = std::numeric_limits<int>::max();

    for (int startCol = 0; startCol < cols; ++startCol) {
        int sum = 0;
        int i = 0, j = startCol;
        while (i < rows && j < cols) {
            sum += matrix[i][j];
            ++i;
            ++j;
        }
        if (sum < minSum) minSum = sum;
    }

    for (int startRow = 1; startRow < rows; ++startRow) {
        int sum = 0;
        int i = startRow, j = 0;
        while (i < rows && j < cols) {
            sum += matrix[i][j];
            ++i;
            ++j;
        }
        if (sum < minSum) minSum = sum;
    }

    return minSum;
}

bool readMatrix(const char* filename, int*& data, int& rows, int& cols, bool useDynamic) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file >> rows >> cols;
    if (file.fail() || rows < 0 || cols < 0) {
        return false;
    }

    if (rows == 0 || cols == 0) {
        data = nullptr;
        return true;
    }

    int totalElements = rows * cols;

    if (!useDynamic && totalElements > 10000) {
        return false;
    }

    if (useDynamic) {
        data = new int[totalElements];
    }

    for (int i = 0; i < totalElements; ++i) {
        file >> data[i];
        if (file.fail()) {
            if (useDynamic) delete[] data;
            return false;
        }
    }

    std::string extra;
    file >> extra;
    if (!file.eof() && !extra.empty()) {
        if (useDynamic) delete[] data;
        return false;
    }

    return true;
}

void writeMatrix(const char* filename, int** matrix, int rows, int cols) {
    std::ofstream file(filename);
    file << rows << " " << cols;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << " " << matrix[i][j];
        }
    }
    file.close();
}

void writeResult(const char* filename, int result) {
    std::ofstream file(filename);
    file << result;
    file.close();
}

}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Not enough arguments";
        return 1;
    }

    if (argc > 4) {
        std::cerr << "Too many arguments";
        return 1;
    }

    int taskNum;
    try {
        taskNum = std::stoi(argv[1]);
    } catch (...) {
        std::cerr << "First parameter is not a number";
        return 1;
    }

    if (taskNum != 1 && taskNum != 2) {
        std::cerr << "First parameter is out of range";
        return 1;
    }

    const char* inputFile = argv[2];
    const char* outputFile = argv[3];

    int rows, cols;
    int* data = nullptr;
    int staticData[10000];
    bool useDynamic = (taskNum == 2);

    if (!useDynamic) {
        data = staticData;
    }

    if (!shaykhraziev::readMatrix(inputFile, data, rows, cols, useDynamic)) {
        std::cerr << "readMatrix failed";
        if (useDynamic && data != nullptr) delete[] data;
        return 2;
    }

    if (rows == 0 || cols == 0) {
        shaykhraziev::writeMatrix(outputFile, nullptr, 0, 0);
        if (useDynamic && data != nullptr) delete[] data;
        return 0;
    }

    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        matrix[i] = &data[i * cols];
    }

    if (taskNum == 1) {
        shaykhraziev::lft_bot_cnt(matrix, rows, cols);
        shaykhraziev::writeMatrix(outputFile, matrix, rows, cols);
    }

    else if (taskNum == 2) {
        int result = shaykhraziev::min_sum_sdg(matrix, rows, cols);
        shaykhraziev::writeResult(outputFile, result);
    }

    delete[] matrix;
    if (useDynamic) delete[] data;

    return 0;
}