#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    clock_t start, finish;
    double  duration;
    /* 测量一个事件持续的时间*/
    start = clock();

    int count = 0;
    int i = 0;
    for(i=0; i<100000; i++){
        int num = 1000;
        int a = 3;
        int b = 4;
        count += a*b;
        while(num>=0){
            num--;
            count++;
            //printf("greater than 10\n");
        }
    }
    printf("Hello world: %d\n", count);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration );
    system("pause");

    return 0;
}

