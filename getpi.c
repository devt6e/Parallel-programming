#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #include "mpi.h"

#define N 1000

int main()
{
    int a = 3.9;
    printf("%d\n", a);
    srand(time(NULL));
    double x, y;
    long long cnt = 0;
    for(int i = 0; i < N; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if(x*x+y*y <= 1) cnt++;
    }
    printf("PI : %.10f\n", 4*cnt/(double)N);
    return 0;
}