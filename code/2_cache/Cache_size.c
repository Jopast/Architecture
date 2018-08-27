#include <time.h>
#include <stdio.h>

#define KB 1024
#define MB KB * KB
#define dataSize 64 * MB
#define minSize 4 * KB

char x[dataSize];
clock_t startTime, endTime;
int  numOfLoop, loopFactor, repeat;
double numBytesToMove, timeTaken;

double getTimeDifference(clock_t start, clock_t end)
{
    return  (end / 1000.0) - (start / 1000.0);
}

double loop(int numOfLoop, int numBytesToMove, int bytesToJump)
{
    int i;
    startTime = clock();

    for (repeat = 1; repeat <= numOfLoop; repeat++) {
        for (i = 0; i < numBytesToMove; i += bytesToJump) {
            x[i]++;
        }
    }

    endTime = clock();

    return  getTimeDifference(startTime, endTime);
}

int main()
{
    FILE *fp = fopen("result.txt", "w");
    double times[20], sizes[20];
    double  sizeFactor, k, notedPercentDrop, timeforFiftyLoops;
    int   threshold, nextCacheLikely = 1;
    int i, j;
    timeforFiftyLoops = loop(50, dataSize, 64);
    loopFactor = (int)1000 / timeforFiftyLoops; //adjust so that each jump will take around 20 seconds

    sizeFactor = 3.0 / 4; k = 8.0 / 9;
    i = 0;
    numBytesToMove = dataSize;

    while (numBytesToMove >= minSize) {
        numOfLoop = (dataSize / numBytesToMove) * loopFactor;
        timeTaken = loop(numOfLoop, (int)numBytesToMove, 64);
        times[i] = timeTaken;
        sizes[i] = numBytesToMove; i++;
        if (numBytesToMove > 2 * MB) {
            numBytesToMove *= sizeFactor;
            sizeFactor *= k;
            k = 1 / k;
        }
        else {
            numBytesToMove /= 2;
        }
        printf("numBytesToMove:%8d, time: %lfs\n", (int)numBytesToMove, timeTaken);
        fprintf(fp, "%8d\t%lf\n", (int)numBytesToMove, timeTaken);
    }
    fclose(fp);
    return 0;
}