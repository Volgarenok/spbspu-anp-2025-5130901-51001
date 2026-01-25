#include <iostream>
#include <fstream>
#include <cstdlib>

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

    return 0;
}
