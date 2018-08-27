#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define Lenght (1<<8<<10<<10)
int loop = (1 << 27);	//循环次数，为了更准确

int main()
{
    clock_t start, end;
    int *array = (int *)malloc(Lenght * sizeof(int));
    for (int ways = 2; ways <= 16; ways += 2) {
        start = clock();
        for (int j = 0; j < loop; ++j)
            for (int i = 0; i < ways; ++i)
                array[(i << 9) << 6] = 0;	//32j L1 cache, 16 int/blocj
        end = clock();
        int total_time = end - start;
        start = clock();		//减去移位运算时间，以得到更准确的结果
        for (int j = 0; j < loop; ++j)
            for (int i = 0; i < ways; ++i)
                (i << 9) << 6;
        end = clock();
        printf("%2d路组相连测试总耗时 %fms\n",
            ways, (double)(total_time - (end - start)) / (ways));
    }
    free(array);
    return 0;
}