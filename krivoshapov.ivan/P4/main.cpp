#include <iostream>
#include <istream>
#include <cstring>

namespace krivoshapov {
    char* rmvVow(const char *src, char *dst, size_t dstSz)
    {
        if (src == nullptr || dst == nullptr || dstSz == 0)
        {
            if (dst != nullptr && dstSz > 0)
            {
                dst[0] ='\0';
            }
            return dst;
        }

        const char vowels[] = "aeiouyAEIOUY";
        size_t writeIdx = 0;

        for (size_t readIdx = 0; src[readIdx] != '\0'; ++readIdx)
        {
            int isVowel = 0;
            char currentChar = src[readIdx];

            for (size_t i = 0; vowels[i] != '\0'; ++i)
            {
                if (currentChar == vowels[i])
                {
                    isVowel = 1;
                    break;
                }
            }

            if (!isVowel)
            {
                if (writeIdx + 1 >= dstSz)
                {
                    break;
                }
                dst[writeIdx] = currentChar;
                ++writeIdx;
            }
        }

        dst[writeIdx] = '\0';
        return dst;
    }

    int seqSym(const char *str, size_t len)
    {
        if (str == nullptr || len < 2)
        {
            return 0;
        }

        for (size_t i = 0; i < len - 1; ++i)
        {
            if (str[i] == str[i + 1])
            {
                return 1;
            }
        }

        return 0;
    }
}

char* readString(size_t& size, size_t& capacity)
{
    const size_t INIT_CAPACITY = 128;
    char* buffer = nullptr;

    if (!std::cin.good() || std::cin.eof()) {
        std::cerr << "Error: No input provided\n";
        return nullptr;
    }

    try
    {
        buffer = new char[INIT_CAPACITY];
    }
    catch (const std::bad_alloc&)
    {
        std::cerr << "Memory allocation failed\n";
        return nullptr;
    }

    capacity = INIT_CAPACITY;
    size = 0;
    char ch;

    if (std::cin.peek() == EOF) {
        delete[] buffer;
        std::cerr << "Error: Empty input\n";
        return nullptr;
    }

    while (std::cin.get(ch) && ch != '\n')
    {
        if (size + 1 >= capacity)
        {
            size_t newCapacity = capacity * 2;
            char* newBuf = nullptr;

            try
            {
                newBuf = new char[newCapacity];
            }
            catch (const std::bad_alloc&)
            {
                delete[] buffer;
                std::cerr << "Memory allocation failed\n";
                return nullptr;
            }
            std::memcpy(newBuf, buffer, size);
            delete[] buffer;
            buffer = newBuf;
            capacity = newCapacity;
        }

        buffer[size] = ch;
        ++size;
    }

    if (size == 0) {
        delete[] buffer;
        std::cerr << "Error: Empty line\n";
        return nullptr;
    }

    buffer[size] = '\0';
    return buffer;
}

int main()
{
    size_t size = 0;
    size_t capacity = 0;

    char *inBuf = readString(size, capacity);
    if(inBuf == nullptr)
    {
        return 1; // Ненулевой код возврата при ошибке
    }

    char *resBuf = nullptr;
    try
    {
        resBuf = new char[capacity];
    }
    catch(const std::bad_alloc&)
    {
        delete[] inBuf;
        std::cerr << "Memory allocation failed\n";
        return 1;
    }

    krivoshapov::rmvVow(inBuf, resBuf, capacity);
    std::cout << resBuf << "\n";

    int seqRes = krivoshapov::seqSym(inBuf, size);
    std::cout << seqRes << "\n";

    delete[] inBuf;
    delete[] resBuf;
    return 0;
}
