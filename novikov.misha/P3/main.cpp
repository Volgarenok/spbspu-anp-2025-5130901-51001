#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// Изменение матрицы по спирали
void spiral(int** m, int r, int c)
{
    int t = r * c;
    int top = 0, bot = r - 1, left = 0, right = c - 1;
    int cnt = 1;
    
    while (cnt <= t)
    {
        for (int i = top; i <= bot && cnt <= t; ++i)
        {
            m[i][left] += cnt;
            ++cnt;
        }
        ++left;
        
        for (int j = left; j <= right && cnt <= t; ++j)
        {
            m[bot][j] += cnt;
            ++cnt;
        }
        --bot;
        
        for (int i = bot; i >= top && cnt <= t; --i)
        {
            m[i][right] += cnt;
            ++cnt;
        }
        --right;
        
        for (int j = right; j >= left && cnt <= t; --j)
        {
            m[top][j] += cnt;
            ++cnt;
        }
        ++top;
    }
}

// Создание сглаженной матрицы
double** smooth(int** m, int r, int c)
{
    double** s = new double*[r];
    for (int i = 0; i < r; ++i)
    {
        s[i] = new double[c];
        for (int j = 0; j < c; ++j)
        {
            s[i][j] = 0.0;
        }
    }
    
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            double sum = 0.0;
            int cnt = 0;
            
            for (int di = -1; di <= 1; ++di)
            {
                for (int dj = -1; dj <= 1; ++dj)
                {
                    if (di == 0 && dj == 0) continue;
                    
                    int ni = i + di;
                    int nj = j + dj;
                    
                    if (ni >= 0 && ni < r && nj >= 0 && nj < c)
                    {
                        sum += m[ni][nj];
                        ++cnt;
                    }
                }
            }
            
            if (cnt > 0)
            {
                s[i][j] = sum / cnt;
            }
        }
    }
    
    return s;
}

// Чтение матрицы
bool read(ifstream& in, int**& m, int& r, int& c)
{
    if (!(in >> r >> c)) return false;
    
    if (r == 0 || c == 0)
    {
        m = nullptr;
        return true;
    }
    
    m = new int*[r];
    for (int i = 0; i < r; ++i)
    {
        m[i] = new int[c];
        for (int j = 0; j < c; ++j)
        {
            if (!(in >> m[i][j])) return false;
        }
    }
    
    return true;
}

// Удаление матрицы
void del(int** m, int r)
{
    if (m)
    {
        for (int i = 0; i < r; ++i)
        {
            delete[] m[i];
        }
        delete[] m;
    }
}

void del(double** m, int r)
{
    if (m)
    {
        for (int i = 0; i < r; ++i)
        {
            delete[] m[i];
        }
        delete[] m;
    }
}

// Запись матрицы
void write(ofstream& out, int** m, int r, int c)
{
    if (r == 0 || c == 0 || !m)
    {
        out << "0 0\n";
        return;
    }
    
    out << r << ' ' << c << '\n';
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            out << m[i][j] << (j == c - 1 ? '\n' : ' ');
        }
    }
}

void writeSmooth(ofstream& out, double** m, int r, int c)
{
    if (r == 0 || c == 0 || !m)
    {
        out << "0 0\n";
        return;
    }
    
    out << r << ' ' << c << '\n';
    out << fixed << setprecision(1);
    
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            out << m[i][j] << (j == c - 1 ? '\n' : ' ');
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <num> <input> <output>\n";
        return 1;
    }
    
    int n = 0;
    try
    {
        n = stoi(argv[1]);
    }
    catch (...)
    {
        cerr << "First parameter is not a number\n";
        return 1;
    }
    
    if (n != 1 && n != 2)
    {
        cerr << "First parameter must be 1 or 2\n";
        return 1;
    }
    
    ifstream in(argv[2]);
    if (!in)
    {
        cerr << "Cannot open input file\n";
        return 2;
    }
    
    int** m = nullptr;
    int r = 0, c = 0;
    
    if (!read(in, m, r, c))
    {
        cerr << "Cannot read matrix from file\n";
        if (m) del(m, r);
        return 2;
    }
    in.close();
    
    ofstream out(argv[3]);
    if (!out)
    {
        cerr << "Cannot open output file\n";
        if (m) del(m, r);
        return 2;
    }
    
    if (n == 1)
    {
        if (r > 0 && c > 0 && m)
        {
            spiral(m, r, c);
        }
        write(out, m, r, c);
    }
    else if (n == 2)
    {
        double** sm = nullptr;
        if (r > 0 && c > 0 && m)
        {
            sm = smooth(m, r, c);
        }
        writeSmooth(out, sm, r, c);
        
        if (sm)
        {
            del(sm, r);
        }
    }
    
    if (m)
    {
        del(m, r);
    }
    
    return 0;
}
