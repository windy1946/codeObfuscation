#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count = 0;

void exe_duration(){
    for(int i=0;i<10000;i++){
        int num = 1000;
	int a = 3;
	int b = 4;
	count += a*b;
	if(count < 10000){
	    count += 100;
	    a = a+3;
	}
	while(num >= 0){
	    num--;
	    count += a;
	}
    }    
}


int main()
{
    clock_t start, finish;
    double  duration;
    /* 测量一个事件持续的时间*/
    start = clock();

    exe_duration();
   
    printf("Hello world: %d\n", count);

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf( "%f seconds\n", duration );

    return 0;
}

