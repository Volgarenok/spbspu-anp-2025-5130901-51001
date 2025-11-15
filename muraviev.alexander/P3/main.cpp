#include <iostream>

namespace muraviev 
{
    void lft_bot_clk(int* matrix, size_t rows, size_t columns) 
    {
        if (rows == 0 || columns == 0) {
            return;
        }

        int left = 0, right = columns - 1;
        int top = 0, bottom = rows - 1;

        int dec = 1;

        while (left <= right && top <= bottom) {
            for(int i = bottom; i >= top; --i) {
                matrix[i * columns + left] -= dec++;
            }
            left++;

            if (left > right) {
                break;
            }
            
            for(int j = left; j <= right; ++j) {
                matrix[top * columns + j] -= dec++;
            }
            top++;

            if (top > bottom) {
                break;
            }

            for(int i = top; i <= bottom; ++i) {
                matrix[i * columns + right] -= dec++;
            }
            right--;

            if (left > right) {
                break;
            }

            for(int j = right; j >= left; --j) {
                matrix[bottom * columns + j] -= dec++;
            }
            bottom--;
        }
    }
}

int main() {
    using std::cout, std::endl;

    int n = 9;
    int matrix[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int rows = 3, cols = 3;

    muraviev::lft_bot_clk(matrix, rows, cols);

    for (int i = 0; i < n; ++i) {
        cout << matrix[i] << " ";
        if ((i + 1) % cols == 0) cout << endl;
    }

    return 0;
}
