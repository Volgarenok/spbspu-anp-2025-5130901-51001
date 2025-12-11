#include <iostream>
#include <fstream>
#include <cstring>

namespace krivoshapov {
    size_t rmVow (const char* src, char* dst, size_t dstSz){
        if (src == nullptr || dst == nullptr || dstSz == 0){
            return 0;
        }

        const char vowels [] = "eiouyAEIOUY";
        const size_t vowelCnt = 12;
        size_t writeIdx = 0;
        size_t srcLen = std::strlen(src);
    }

}

int main(){
    
}