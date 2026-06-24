#include <iostream>

using namespace std;

// [UNC-SYM]
char* findUnique(const char* s1, const char* s2)
{
    int size = 1;

    for (int i = 0; s1[i] != '\0'; ++i)
    {
        char c = s1[i];
        bool found = false;

        int j = 0;
        while (s2[j] != '\0')
        {
            if (s2[j] == c)
            {
                found = true;
                break;
            }
            ++j;
        }

        if (!found) ++size;
    }

    for (int i = 0; s2[i] != '\0'; ++i)
    {
        char c = s2[i];
        bool found = false;

        int j = 0;
        while (s1[j] != '\0')
        {
            if (s1[j] == c)
            {
                found = true;
                break;
            }
            ++j;
        }

        if (!found) ++size;
    }

    char* out = (char*)malloc(size);

    int k = 0;

    for (int i = 0; s1[i] != '\0'; ++i)
    {
        char c = s1[i];
        bool found = false;

        int j = 0;
        while (s2[j] != '\0')
        {
            if (s2[j] == c)
            {
                found = true;
                break;
            }
            ++j;
        }

        if (!found)
        {
            out[k] = c;
            ++k;
        }
    }

    for (int i = 0; s2[i] != '\0'; ++i)
    {
        char c = s2[i];
        bool found = false;

        int j = 0;
        while (s1[j] != '\0')
        {
            if (s1[j] == c)
            {
                found = true;
                break;
            }
            ++j;
        }

        if (!found)
        {
            out[k] = c;
            ++k;
        }
    }

    out[k] = '\0';
    return out;
}

// [SEQ-SYM]
int hasSequence(const char* s)
{
    int i = 0;
    while (s[i] != '\0' && s[i+1] != '\0')
    {
        if (s[i] == s[i+1])
        {
            return 1;
        }
        ++i;
    }
    return 0;
}

int main()
{
    return 0;
}
