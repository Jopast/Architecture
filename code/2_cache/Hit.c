#include <stdio.h>  
#include <stdlib.h>  
#include <windows.h>  
__int64 __declspec(naked) read_time_stamp_counter()
{
    __asm cpuid;//使用汇编指令测试时间精度更高
    __asm rdtsc;
    __asm ret;
}
#define SIZE_1KB (1024)
#define SIZE_1MB (1024*1024)
#define NUMBER 128*SIZE_1MB 
int main()
{
    long long ticks_1, ticks_2;
    long perus;
    int i, step, s;;
    int * a;
    ticks_1 = (double)read_time_stamp_counter();
    Sleep(1000);
    ticks_2 = (double)read_time_stamp_counter();
    perus = (ticks_2 - ticks_1) / (1000 * 1000);//测试cpu当前时钟频率

    //测试命中时间
    a = (int *)malloc(NUMBER*sizeof(int));
    int len = SIZE_1MB;//访问的数组大小
    //先将数组取到cache中
    for (i = 0; i < len / 16; i++) {
        a[(i * 16) & (len)]++;//linesize 是64B，16 means linesizesizeof(int) = 16
    }
    ticks_1 = (double)read_time_stamp_counter();

    for (i = 0; i < SIZE_1MB; i++) {
        a[(i * 16) & (len)]++;//*16 means linesizesizeof(int) = 16
    }
    ticks_2 = (double)read_time_stamp_counter();
    printf("命中时间为:%.8f us\n", (double)(ticks_2 - ticks_1) / perus / SIZE_1MB);

    return 0;
}