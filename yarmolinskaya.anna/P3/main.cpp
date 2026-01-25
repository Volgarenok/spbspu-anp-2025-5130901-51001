#include <iostream>
#include <fstream>
#include <cstdlib>

const int MAX_ELEMENTS = 10000;

bool parseInt(const char* s, int& value) {
    if (s == nullptr || *s == '\0')
        return false;

    char* end = nullptr;
    long v = std::strtol(s, &end, 10);

    if (*end != '\0')
        return false;

    value = static_cast<int>(v);
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Invalid number of arguments\n";
        return 1;
    }

    int num = 0;
    if (!parseInt(argv[1], num) || num != 1) {
        std::cerr << "Invalid task number\n";
        return 1;
    }

    std::ifstream fin(argv[2]);
    if (!fin) {
        std::cerr << "Cannot open input file\n";
        return 2;
    }

    std::ofstream fout(argv[3]);
    if (!fout) {
        std::cerr << "Cannot open output file\n";
        return 2;
    }

    int rows = 0, cols = 0;
    if (!(fin >> rows >> cols) || rows < 0 || cols < 0) {
        std::cerr << "Invalid matrix format\n";
        return 2;
    }

    int total = rows * cols;
    if (total > MAX_ELEMENTS) {
        std::cerr << "Matrix is too large\n";
        return 2;
    }

    int matrix[MAX_ELEMENTS];
    for (int i = 0; i < total; ++i) {
        if (!(fin >> matrix[i])) {
            std::cerr << "Invalid matrix data\n";
            return 2;
        }
    }

    int k = (rows < cols) ? rows : cols;
    int result = 0;

    for (int j = 0; j < k; ++j) {
        bool ok = true;
        for (int i = 0; i + 1 < k; ++i) {
            if (matrix[i * cols + j] == matrix[(i + 1) * cols + j]) {
                ok = false;
                break;
            }
        }
        if (ok)
            ++result;
    }

    fout << result;
    return 0;
}
