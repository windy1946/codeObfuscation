#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <time.h>

#ifndef UTILTOOL
#define UTILTOOL

class UtilTool{
public:

    static const int MAX_NUM = 100;
    static int getrandnum(int num){
        if(num <= 0){
            return 0;
        }
        srand((unsigned)time(NULL));
        return (int)(rand()%num);
    }
};

#endif