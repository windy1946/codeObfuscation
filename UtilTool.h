#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <time.h>

#ifndef UTILTOOL
#define UTILTOOL

static long myoffsetnum = 44;

class UtilTool{
public:

    static const int MAX_NUM = 100;
    
    static int getrandnum(int num){
        if(num <= 0){
            return 0;
        }
        myoffsetnum++;

        srand(time(NULL));

        return (int)((rand()+myoffsetnum)%num);
    }

};

#endif