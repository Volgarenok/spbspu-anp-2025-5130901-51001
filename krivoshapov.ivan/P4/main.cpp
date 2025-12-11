#include <iostream>
#include <istream>
#include <cstring>

namespace Krivoshapov
{
    size_t rmvVow(const char* src, char* dst, size_t dstSz)
    {
        if (src == nullptr || dst == nullptr || dstSz == 0) {
            return 0;
        }

        const char vowels[] = "aeiouyAEIOUY";
        size_t writeIdx = 0;
        size_t srcLen = std::strlen(src);

        for (size_t readIdx = 0; readIdx < srcLen; ++readIdx) {
            int isVowel = 0;
            char currentChar = src[readIdx];

            for (size_t i = 0; vowels[i] != '\0'; ++i) {
                if (currentChar == vowels[i]) {
                    isVowel = 1;
                    break;
                }
            }

            if (!isVowel) {
                if (writeIdx + 1 >= dstSz) {
                    break;
                }
                dst[writeIdx] = currentChar;
                ++writeIdx;
            }
        }

        dst[writeIdx] = '\0';
        return writeIdx;
    }

    int seqSym(const char* str)
    {
        if (str == nullptr) {
            return 0;
        }

        size_t len = std::strlen(str);
        if (len < 2) {
            return 0;
        }

        for (size_t i = 0; i < len - 1; ++i) {
            if (str[i] == str[i + 1]) {
                return 1;
            }
        }

        return 0;
    }
}

int main()
{
    const size_t initSz = 128;
    char* inBuf = nullptr;
    char* resBuf = nullptr;

    try {
        inBuf = new char[initSz];
    } catch (std::bad_alloc&) {
        std::cerr << "Memory allocation failed" << std::endl;
        return 1;
    }

    size_t bufSz = initSz;
    size_t strLen = 0;
    char ch;

    while (std::cin.get(ch) && ch != '\n') {
        if (strLen + 1 >= bufSz) {
            size_t newSz = bufSz * 2;
            char* newBuf = nullptr;

            try {
                newBuf = new char[newSz];
            } catch (std::bad_alloc&) {
                delete[] inBuf;
                std::cerr << "Memory allocation failed" << std::endl;
                return 1;
            }

            std::memcpy(newBuf, inBuf, strLen);
            delete[] inBuf;
            inBuf = newBuf;
            bufSz = newSz;
        }

        inBuf[strLen] = ch;
        ++strLen;
    }

    inBuf[strLen] = '\0';

    try {
        resBuf = new char[bufSz];
    } catch (std::bad_alloc&) {
        delete[] inBuf;
        std::cerr << "Memory allocation failed" << std::endl;
        return 1;
    }

    Krivoshapov::rmvVow(inBuf, resBuf, bufSz);
    std::cout << resBuf << std::endl;

    int seqRes = Krivoshapov::seqSym(inBuf);
    std::cout << seqRes << std::endl;

    delete[] inBuf;
    delete[] resBuf;
    return 0;
}
