#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <cmath>

const int MAX_ELEMENTS = 10000;
const int MAX_ROWS = 100;
const int MAX_COLS = 100;

bool isNumber(const std::string& str) {
    if (str.empty()) return false;
    
    for (size_t i = 0; i < str.length(); i++) {
        if (i == 0 && str[i] == '-') {
            if (str.length() == 1) return false;
            continue;
        }
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

double roundToTenth(double value) {
    return std::round(value * 10.0) / 10.0;
}

void smoothMatrix(double** matrix, int rows, int cols, double** result) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double sum = 0.0;
            int count = 0;
            
            if (i > 0) {
                sum += matrix[i-1][j];
                count++;
            }
            if (i < rows-1) {
                sum += matrix[i+1][j];
                count++;
            }
            if (j > 0) {
                sum += matrix[i][j-1];
                count++;
            }
            if (j < cols-1) {
                sum += matrix[i][j+1];
                count++;
            }
            
            result[i][j] = (count > 0) ? roundToTenth(sum / count) : 0.0;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Неверное количество аргументов" << std::endl;
        return 1;
    }
    
    std::string modeStr = argv[1];
    if (!isNumber(modeStr) || (modeStr != "1" && modeStr != "2")) {
        std::cerr << "Первый параметр должен быть 1 или 2" << std::endl;
        return 1;
    }
    int mode = std::stoi(modeStr);
    
    std::ifstream inputFile(argv[2]);
    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть входной файл" << std::endl;
        return 2;
    }
    
    int rows, cols;
    if (!(inputFile >> rows >> cols)) {
        std::cerr << "Не удалось прочитать размеры матрицы" << std::endl;
        inputFile.close();
        return 2;
    }
    
    if (rows < 0 || cols < 0) {
        std::cerr << "Размеры матрицы должны быть неотрицательными" << std::endl;
        inputFile.close();
        return 2;
    }
    
    if (mode == 1 && rows * cols > MAX_ELEMENTS) {
        std::cerr << "Матрица слишком большая для статического режима" << std::endl;
        inputFile.close();
        return 2;
    }
    
    if (rows == 0 || cols == 0) {
        inputFile.close();
        
        std::ofstream outputFile(argv[3]);
        if (!outputFile.is_open()) {
            std::cerr << "Не удалось открыть выходной файл" << std::endl;
            return 2;
        }
        
        outputFile << rows << " " << cols;
        outputFile.close();
        return 0;
    }
    
    double** matrix;
    double** smoothed;
    
    if (mode == 1) {
        if (rows > MAX_ROWS || cols > MAX_COLS) {
            std::cerr << "Размеры матрицы превышают максимум для статического режима" << std::endl;
            inputFile.close();
            return 2;
        }
        
        matrix = new double*[rows];
        smoothed = new double*[rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new double[cols];
            smoothed[i] = new double[cols];
        }
    } else {
        matrix = new double*[rows];
        smoothed = new double*[rows];
        for (int i = 0; i < rows; i++) {
            matrix[i] = new double[cols];
            smoothed[i] = new double[cols];
        }
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!(inputFile >> matrix[i][j])) {
                std::cerr << "Не удалось прочитать элемент матрицы" << std::endl;
                
                for (int k = 0; k < rows; k++) {
                    delete[] matrix[k];
                    delete[] smoothed[k];
                }
                delete[] matrix;
                delete[] smoothed;
                inputFile.close();
                return 2;
            }
        }
    }
    inputFile.close();
    
    int extra;
    if (inputFile >> extra) {
        std::cerr << "Лишние данные во входном файле" << std::endl;
        
        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
            delete[] smoothed[i];
        }
        delete[] matrix;
        delete[] smoothed;
        return 2;
    }
    
    smoothMatrix(matrix, rows, cols, smoothed);
    
    std::ofstream outputFile(argv[3]);
    if (!outputFile.is_open()) {
        std::cerr << "Не удалось открыть выходной файл" << std::endl;
        
        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
            delete[] smoothed[i];
        }
        delete[] matrix;
        delete[] smoothed;
        return 2;
    }
    
    outputFile << std::fixed << std::setprecision(1);
    outputFile << rows << " " << cols;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outputFile << " " << smoothed[i][j];
        }
    }
    outputFile.close();
    
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
        delete[] smoothed[i];
    }
    delete[] matrix;
    delete[] smoothed;
    
    return 0;
}
