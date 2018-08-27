#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define ID 5
FILE *result;
int Block_array[5] = {8, 16, 32, 64, 128};
int Block = 8;
int N_array[5] = {128, 256, 512, 1024, 2048};
int n_array[5] = {4096, 512, 64,    8,    1};

/*��ʼ����ά N x N ���飬A��BΪ�������CΪȫ0*/
void mat_init(int *A, int *B, int *C, int N)
{
    srand((int)time(0));
    for (int i = 0; i < N * N; i++)
    {
        A[i] = rand();
        B[i] = rand();
        C[i] = 0;
    }
}

/*������ά����ռ䣬�ֱ𶼴�src��dest*/
void copy_mem(int *dest1, int *src1, int *dest2, int *src2, int *dest3, int *src3, int N)
{
    memcpy(dest1, src1, N * N * sizeof(int));
    memcpy(dest2, src2, N * N * sizeof(int));
    memcpy(dest3, src3, N * N * sizeof(int));
}

/*�ͷŶ�ά����ռ�*/
void free_mem(int *A, int *B, int *C)
{
    free(A);
    free(B);
    free(C);
}

/*�Ƚ����������Ƿ���ͬ����ͬ����1����ͬ����0*/
int not_equal(int *A, int *B, int N)
{
    for (int i = 0; i < N * N; i++)
        if (A[i] != B[i])
           return 1;

    return 0;
}

/*�ֱ�����Ӧ�������о���˷���������mat_mul��������ʱ��*/
void MAT_MUL(int *A, int *B, int *C, int N, int n, int func_idx, void (*mat_mul)(int *dest, int *src1, int*src2, int N, int *temp))
{
    int *temp = NULL;
    temp = (int*)malloc(N * N * sizeof(int));

    clock_t start, end;
    double duration;
    start = clock();
    for (int i = 0; i < n; i++)
    {
        mat_mul(C, A, B, N, temp);
        mat_mul(B, A, C, N, temp);
        mat_mul(A, B, C, N, temp);
    }
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("method %d\tN=%d\tn=%d\tduration  %f\n", func_idx, N, n, duration);
    fprintf(result, "%d,%d,%d,%f\n", func_idx, N, n, duration);

    free(temp);
}

/*�����ľ���˷�*/
void M0(int *dest, int *src1, int *src2, int N, int *temp)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                dest[i * N + j] += src1[i * N + k] * src2[k * N +j];
}

/*��src2������ת�ã�Ȼ����������������������*/
void M1(int *dest, int *src1, int *src2, int N, int *temp)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            temp[i * N + j] = src2[j * N + i];

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                dest[i * N + j] += src1[i * N + k] * temp[j * N + k];
}

/*��src2������ת�ã��Ƚ�src1��src2ͬһ�е����ݷֿ�*/
void M2(int *dest, int *src1, int *src2, int N, int *temp)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            temp[i * N + j] = src2[j * N + i];
   
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int kk = 0; kk < N; kk += Block)
            {
                int r = 0;
                int end_k = (kk + Block)>N ? N : (kk + Block);
                for (int k = kk; k < end_k; k++)
                {
                    r += src1[i * N + k] * temp[j * N + k];
                }
                dest[i * N + j] += r;
            }
        }
    }
}

/*��src2������ת�ã��Ƚ�src1��src2ͬһ�е����ݷֿ飬�ٽ�src2ת�ú�����ݰ��зֿ�*/
void M3(int *dest, int *src1, int *src2, int N, int *temp)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            temp[i * N + j] = src2[j * N + i];

    for (int i = 0; i < N; i++)
    {
        for (int jj = 0; jj < N; jj += Block)
        {
            for (int kk = 0; kk < N; kk += Block)
            {
                int end_j = (jj + Block)>N ? N : (jj + Block);
                for (int j = jj; j < end_j; j++)
                {
                    int r = 0;
                    int end_k = (kk + Block)>N ? N : (kk + Block);
                    for (int k = kk; k < end_k; k++)
                    {
                        r += src1[i * N + k] * temp[j * N + k];
                    }
                    dest[i * N + j] += r;
                }
            }
        }
    }
}

/*��src2������ת�ã��Ƚ�src1��src2ͬһ�е����ݷֿ飬�ٽ�src2ת�ú�����ݰ��зֿ飬���src1�����ݰ��зֿ�*/
void M4(int *dest, int *src1, int *src2, int N, int *temp)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            temp[i * N + j] = src2[j * N + i];

    for (int ii = 0; ii < N; ii += Block)
    {
        for (int jj = 0; jj < N; jj += Block)
        {
            for (int kk = 0; kk < N; kk += Block)
            {
                int end_i = (ii + Block)>N ? N : (ii + Block);
                for (int i = ii; i<end_i; i++)
                {
                    int end_j = (jj + Block)>N ? N : (jj + Block);
                    for (int j = jj; j < end_j; j++)
                    {
                        int r = 0;
                        int end_k = (kk + Block)>N ? N : (kk + Block);
                        for (int k = kk; k < end_k; k++)
                        {
                            r += src1[i * N + k] * temp[j * N + k];
                        }
                        dest[i * N + j] += r;
                    }
                }
            }
        }
    }
}



int main()
{
    result = fopen("result.csv", "w");
    fprintf(result, "%s,%s,%s,%s\n", "method", "N", "n", "duration");
    for (int tt =0; tt < 5; tt++){
        Block = Block_array[tt];
        for (int idx = 0; idx < ID; idx++)
        {
            int N = N_array[idx];
            int *A, *B, *C;
            A = (int *)malloc(N * N * sizeof(int));
            B = (int *)malloc(N * N * sizeof(int));
            C = (int *)malloc(N * N * sizeof(int));
            mat_init(A, B, C, N);

            /*��1�־���˷��Ľ�������ں��Ժ�ľ���˷��Ľ�����Ƚ�*/
            int *A_copy = NULL, *B_copy = NULL, *C_copy = NULL;
            A_copy = (int *)malloc(N * N * sizeof(int));
            B_copy = (int *)malloc(N * N * sizeof(int));
            C_copy = (int *)malloc(N * N * sizeof(int));

            /*��2�ּ��Ժ�ľ���˷��Ľ��*/
            int *A_copy_2 = NULL, *B_copy_2 = NULL, *C_copy_2 = NULL;
            A_copy_2 = (int *)malloc(N * N * sizeof(int));
            B_copy_2 = (int *)malloc(N * N * sizeof(int));
            C_copy_2 = (int *)malloc(N * N * sizeof(int));

            {
                int n = n_array[idx];

                /* M0 */
                copy_mem(A_copy, A, B_copy, B, C_copy, C, N);
                MAT_MUL(A_copy, B_copy, C_copy, N, n, 0, &M0);

                /* M1 */
                copy_mem(A_copy_2, A, B_copy_2, B, C_copy_2, C, N);
                MAT_MUL(A_copy_2, B_copy_2, C_copy_2, N, n, 1, &M1);
                if (not_equal(A_copy_2, A_copy, N) || not_equal(B_copy_2, B_copy, N) || not_equal(C_copy_2, C_copy, N))
                {
                    printf("��1�־���˷��������\n");
                }

                /* M2 */
                copy_mem(A_copy_2, A, B_copy_2, B, C_copy_2, C, N);
                MAT_MUL(A_copy_2, B_copy_2, C_copy_2, N, n, 2, &M2);
                if (not_equal(A_copy_2, A_copy, N) || not_equal(B_copy_2, B_copy, N) || not_equal(C_copy_2, C_copy, N))
                {
                    printf("��2�־���˷��������\n");
                }

                /* M3 */
                copy_mem(A_copy_2, A, B_copy_2, B, C_copy_2, C, N);
                MAT_MUL(A_copy_2, B_copy_2, C_copy_2, N, n, 3, &M3);
                if (not_equal(A_copy_2, A_copy, N) || not_equal(B_copy_2, B_copy, N) || not_equal(C_copy_2, C_copy, N))
                {
                    printf("��3�־���˷��������\n");
                }

                /* M4 */
                copy_mem(A_copy_2, A, B_copy_2, B, C_copy_2, C, N);
                MAT_MUL(A_copy_2, B_copy_2, C_copy_2, N, n, 4, &M4);
                if (not_equal(A_copy_2, A_copy, N) || not_equal(B_copy_2, B_copy, N) || not_equal(C_copy_2, C_copy, N))
                {
                    printf("��4�־���˷��������\n");
                }
            }

            free_mem(A_copy_2, B_copy_2, C_copy_2);
            free_mem(A_copy, B_copy, C_copy);
            free_mem(A, B, C);
        }
    }
    fclose(result);

    system("pause");
    return 0;
}