#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <time.h>

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