#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>

const size_t MAX_MATRIX_SIZE = 10000;

namespace volkovich
{
    void spiral(int (&matrix)[MAX_MATRIX_SIZE], int rows, int columns);
    int diagonal(int *matrix, int rows, int columns);
}

void volkovich::spiral(int (&matrix)[MAX_MATRIX_SIZE], int rows, int columns)
{
    int top{}, bottom = rows - 1, left{}, right = columns - 1;
    int i{};
    int minus_value = 1;
    while (top <= bottom)
    {
        for (i = top * columns + left; i <= top * columns + right; i++)
        {
            matrix[i] -= minus_value;
            minus_value++;
        }
        if (top == bottom)
        {
            break;
        }
        for (i = (top + 1) * columns + right; i <= (bottom + 1) * columns - left; i += columns)
        {
            matrix[i] -= minus_value;
            minus_value++;
        }
        for (i = (bottom * columns) + right - 1; i >= left + bottom * columns; i--)
        {
            matrix[i] -= minus_value;
            minus_value++;
        }
        for (i = (bottom - 1) * columns + left; i >= (top + 1) * columns + left; i -= columns)
        {
            matrix[i] -= minus_value;
            minus_value++;
        }
        top++;
        bottom--;
        right--;
        left++;
    }
}

int volkovich::diagonal(int *matrix, int rows, int columns)
{
    if (rows==0 && columns==0) {
        return 0;
    }
    int res = INT_MAX;
    for (int s = 0; s <= rows + columns - 2; s++)
    {
        int sum=0;
        for (int i = 0; i < rows; i++)
        {
            int j = s - i;
            if (j>=0 && j<columns) {
                sum+=matrix[i*columns+j];
            }
        }
        res = std::min(sum, res);
    }
    return res;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Not enough arguments";
        return 1;
    }
    if (argc > 4)
    {
        std::cerr << "Too many arguments";
        return 1;
    }
    if ((strcmp(argv[1], "1") && strcmp(argv[1], "2")))
    {
        std::cerr << "First argument out of range";
        return 1;
    }
    std::ifstream input(argv[2]);
    if (!input.is_open())
    {
        std::cerr << "Cannot open file";
        return 1;
    }

    int rows{}, columns{};

    if (!(input >> rows >> columns)) {
        return 1;
    }
    if (!strcmp(argv[1], "1"))
    {
        int matrix[MAX_MATRIX_SIZE];
        for (int i{}; i < rows * columns; i++)
        {
            input >> matrix[i];
        }
        volkovich::spiral(matrix, rows, columns);
        std::ofstream output(argv[3]);
        output << rows << ' ' << columns;
        for (int i = 0; i < rows * columns; i++)
        {
            output << ' ' << matrix[i];
        }
        output.close();
    }

    else
    {
        int *matrix = new int[rows * columns];
        for (int i{}; i < rows * columns; i++)
        {
            input >> matrix[i];
        }

        int res = volkovich::diagonal(matrix, rows, columns);
        std::ofstream output(argv[3]);
        output << res;
        output.close();
        delete[] matrix;
    }
    input.close();
    return 0;
}
