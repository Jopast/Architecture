#include <time.h>
#include <stdio.h>

#define KB 1024
#define MB KB * KB
#define dataSize 64 * MB
#define minSize 4 * KB

char x[dataSize];
clock_t startTime, endTime;
int blockSize;
double timeTaken, prevTime;

double getTimeDifference(clock_t start, clock_t end)
{
    return  (end / 1000.0) - (start / 1000.0);
}

double loop(int numOfLoop, int numBytesToMove, int bytesToJump)
{
    int i;
    startTime = clock();

    for (int repeat = 1; repeat <= numOfLoop; repeat++) {
        for (i = 0; i < numBytesToMove; i += bytesToJump) {
            x[i]++;
        }
    }

    endTime = clock();

    return  getTimeDifference(startTime, endTime);
}

int main()
{
    int i, j, currentBlock, sizes[8];
    double ratio = 0;  currentBlock = 1;
    timeTaken = 0, prevTime = 0;
    int numOfLoop = 20;
    i = 0;
    while (currentBlock <= 256) {
        prevTime = timeTaken;
        timeTaken = loop(numOfLoop, dataSize, currentBlock);

        printf("当步长为%d时，耗时为:%lfs\n", currentBlock, timeTaken);
        sizes[i++] = currentBlock;
        currentBlock *= 2;
        numOfLoop *= 2;
        blockSize = currentBlock;
    }
    return 0;
}