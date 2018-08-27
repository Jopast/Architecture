#include <stdio.h>  
#include <stdlib.h>  
#include <windows.h>  
__int64 __declspec(naked) read_time_stamp_counter()
{
    __asm cpuid;//ʹ�û��ָ�����ʱ�侫�ȸ���
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
    perus = (ticks_2 - ticks_1) / (1000 * 1000);//����cpu��ǰʱ��Ƶ��

    //��������ʱ��
    a = (int *)malloc(NUMBER*sizeof(int));
    int len = SIZE_1MB;//���ʵ������С
    //�Ƚ�����ȡ��cache��
    for (i = 0; i < len / 16; i++) {
        a[(i * 16) & (len)]++;//linesize ��64B��16 means linesizesizeof(int) = 16
    }
    ticks_1 = (double)read_time_stamp_counter();

    for (i = 0; i < SIZE_1MB; i++) {
        a[(i * 16) & (len)]++;//*16 means linesizesizeof(int) = 16
    }
    ticks_2 = (double)read_time_stamp_counter();
    printf("����ʱ��Ϊ:%.8f us\n", (double)(ticks_2 - ticks_1) / perus / SIZE_1MB);

    return 0;
}