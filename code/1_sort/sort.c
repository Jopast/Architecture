/*****************************************************
File name：Quicksort
Author：Xiayaxian    Version:1.0    Date: 2017/11/07
Description: 对数组进行快速排序
Funcion List: 实现快速排序算法
*****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>  
#include <stdint.h>

/**************************************************
*函数名：display
*作用：打印数组元素
*参数：array - 打印的数组，maxlen - 数组元素个数
*返回值：无
**************************************************/
void display(int array[], int maxlen)
{
	int i;

	for (i = 0; i < maxlen; i++)
	{
		printf("%-10d", array[i]);
	}
	printf("\n");

	return;
}

/********************************
*函数名：swap
*作用：交换两个数的值
*参数：交换的两个数
*返回值：无
********************************/
void swap(int *a, int *b)
{
	int temp;

	temp = *a;
	*a = *b;
	*b = temp;

	return;
}
/********************************
*函数名：random
*作用：生成随机数
*参数：数组
*返回值：无
********************************/
void random(int array[], uint64_t len)
{   
	//int array[BUF_SIZE];
	srand((int)time(0));
    for (int i = 0; i<len; i++)
		array[i]=rand();
	//random(array);
}
/************************************
*函数名：quicksort
*作用：快速排序算法
*参数：
*返回值：无
************************************/
//void quicksort(int array[], int maxlen, int begin, int end)
//{
//	int i, j;
//
//	if (begin < end)
//	{
//		i = begin + 1;  // 将array[begin]作为基准数，因此从array[begin+1]开始与基准数比较！
//		j = end;        // array[end]是数组的最后一位
//
//		while (i < j)
//		{
//			if (array[i] > array[begin])  // 如果比较的数组元素大于基准数，则交换位置。
//			{
//				swap(&array[i], &array[j]);  // 交换两个数
//				j--;
//			}
//			else
//			{
//				i++;  // 将数组向后移一位，继续与基准数比较。
//			}
//		}
//
//		/* 跳出while循环后，i = j。
//		* 此时数组被分割成两个部分  -->  array[begin+1] ~ array[i-1] < array[begin]
//		*                           -->  array[i+1] ~ array[end] > array[begin]
//		* 这个时候将数组array分成两个部分，再将array[i]与array[begin]进行比较，决定array[i]的位置。
//		* 最后将array[i]与array[begin]交换，进行两个分割部分的排序！以此类推，直到最后i = j不满足条件就退出！
//		*/
//
//		if (array[i] >= array[begin])  // 这里必须要取等“>=”，否则数组元素由相同的值时，会出现错误！
//		{
//			i--;
//		}
//
//		swap(&array[begin], &array[i]);  // 交换array[i]与array[begin]
//
//		quicksort(array, maxlen, begin, i);
//		quicksort(array, maxlen, j, end);
//	}
//}

void quicksort(int* A, int n)
{
    typedef struct
    {
        int beg;
        int end;
    }sbe;

    sbe* arr = (sbe *)malloc(n * sizeof(sbe));
	int  m;
	int p;
	int beg, end, i, j;

	m = 0;
	arr[0].beg = 0;
	arr[0].end = n - 1;

	for (;;)
	{
		if (m < 0)
			break;
		beg = arr[m].beg;
		end = arr[m--].end;

		if (beg >= end)
			continue;

		// partition   
		p = A[beg];
		i = beg;
		j = end + 1;
		for (;;)
		{
			while (++i <= end && A[i] < p);  // ! while( A[++i] < p ); error  
			while (A[--j] > p);
			if (i > j)
				break;
			swap(&A[i], &A[j]);
		}
		swap(&A[j], &A[beg]);

		arr[++m].beg = beg;
		arr[m].end = j - 1;
		arr[++m].beg = j + 1;
		arr[m].end = end;
	}

	free(arr);
}
/************************************
*函数名：insertion_sort
*作用：插入排序算法
*参数：
*返回值：无
************************************/
void insertion_sort(int *a, int n)//a为数组，n为数组个数  
{
	int i, j, key;

	for (j = 1; j < n; j++) {
		key = a[j];
		i = j - 1;
		while (i >= 0 && a[i] > key) {
			a[i + 1] = a[i];
			i--;
		}
		a[i + 1] = key;
	}
}
/************************************
*函数名：merge_sort
*作用：归并排序算法
*参数：
*返回值：无
************************************/
void merge_sort(int *list, int length)
{
	int i, left_min, left_max, right_min, right_max, next;
	//int tmp[BUF_SIZE];
	int *tmp = (int*)malloc(sizeof(int) * length);

	if (tmp == NULL)
	{
		fputs("Error: out of memory\n", stderr);
		abort();
	}

	for (i = 1; i < length; i *= 2) // i为步长，1,2,4,8……
	{
		for (left_min = 0; left_min < length - i; left_min = right_max)
		{
			right_min = left_max = left_min + i;
			right_max = left_max + i;

			if (right_max > length)
				right_max = length;

			next = 0;
			while (left_min < left_max && right_min < right_max)
				tmp[next++] = list[left_min] > list[right_min] ? list[right_min++] : list[left_min++];

			while (left_min < left_max)
				list[--right_min] = list[--left_max];

			while (next > 0)
				list[--right_min] = tmp[--next];
		}
	}

	free(tmp);

}

// 主函数
int main()
{
    FILE *result = fopen("result.txt", "w");
    uint64_t max_buffer_size = 1024*1024*16;
    uint64_t min_buffer_size = 1024;
	int n =5;                         //测试次数
	int *Q_array = NULL, *I_array = NULL, *M_array = NULL;
    Q_array = (int *)malloc(sizeof(int) * max_buffer_size);
    I_array = (int *)malloc(sizeof(int) * max_buffer_size);
    M_array = (int *)malloc(sizeof(int) * max_buffer_size);

	//printf("排序前的数组\n");
	//display(Q_array, maxlen);
	clock_t Q_start, Q_finish, I_start, I_finish,M_start, M_finish;
	double Q_duration, I_duration = 0, M_duration = 0, Q_sum = 0, I_sum = 0, M_sum = 0, Q_avg = 0, I_avg = 0, M_avg = 0;
	double duration;

    printf("start testing......\n");
    int count = 0;
    printf("%d%%\r", count * 100 / 8);
    for (int arrlen = min_buffer_size; arrlen <= max_buffer_size; arrlen *= 4) {
        int buffer_len = arrlen / 1024;
        if (buffer_len<1024) {
            fprintf(result, "%dK数组排序时间:\n", buffer_len);
        }
        else {
            buffer_len /= 1024;
            fprintf(result, "%dM数组排序时间:\n", buffer_len);
        }
        for (int time = 1; time <= n; time++) {
            random(Q_array, arrlen);
            memcpy(I_array, Q_array, sizeof(int) * max_buffer_size);
            memcpy(M_array, Q_array, sizeof(int) * max_buffer_size);
            Q_duration = 0;
            I_duration = 0;
            M_duration = 0;

            Q_start = clock();
            quicksort(Q_array, arrlen);  // 快速排序
            Q_finish = clock();
            Q_duration = (double)(Q_finish - Q_start) / CLOCKS_PER_SEC;

            if (arrlen < 64000) {
                I_start = clock();
                insertion_sort(I_array, arrlen);
                I_finish = clock();
                I_duration = (double)(I_finish - I_start) / CLOCKS_PER_SEC;
            }

            M_start = clock();
            merge_sort(M_array, arrlen);
            M_finish = clock();
            M_duration = (double)(M_finish - M_start) / CLOCKS_PER_SEC;

            duration = (double)(M_finish - Q_start) / CLOCKS_PER_SEC;

            //display(Q_array, maxlen);
            fprintf(result, "第%d次快速排序用时： %f seconds\n", time, Q_duration);
            fprintf(result, "第%d次插入排序用时： %f seconds\n", time, I_duration);
            fprintf(result, "第%d次归并排序用时： %f seconds\n", time, M_duration);
            fprintf(result, "用时： %f seconds\n", duration);
            fprintf(result, "\n");
        }
        fprintf(result, "\n");
        count++;
        printf("%d%%\r", count*100/8);
    }
    printf("\nend\n");
	free(Q_array);
	free(I_array);
	free(M_array);
    fclose(result);
	system("pause");
	return 0;
}