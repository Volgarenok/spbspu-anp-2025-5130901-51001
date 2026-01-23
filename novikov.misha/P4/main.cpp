#include <iostream>

using namespace std;

// [UNC-SYM]
void findUnique(const char* s1, const char* s2, char* out)
{
    int k = 0;
    
    for (int i = 0; s1[i]; ++i)
    {
        char c = s1[i];
        bool found = false;
        
        for (int j = 0; s2[j]; ++j)
        {
            if (s2[j] == c)
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            out[k] = c;
            ++k;
        }
    }
    
    for (int i = 0; s2[i]; ++i)
    {
        char c = s2[i];
        bool found = false;
        
        for (int j = 0; s1[j]; ++j)
        {
            if (s1[j] == c)
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            out[k] = c;
            ++k;
        }
    }
    
    out[k] = '\0';
}

// [SEQ-SYM]
int hasSequence(const char* s)
{
    for (int i = 0; s[i] && s[i+1]; ++i)
    {
        if (s[i] == s[i+1])
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    return 0;
}
