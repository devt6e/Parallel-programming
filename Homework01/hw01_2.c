#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[])
{
    //argv : cmd DIM ITER stride
    if(argc != 4)
    {
        printf("plz try again\n");
        return 0;
    }
    int DIM = atoi(argv[1]);
    int ITER = atoi(argv[2]);
    int stride = atoi(argv[3]);    
    double Nref = 2 * (DIM+stride-1) / stride;
    double Nops = 2 * Nref * ITER;

    double *A, *B, *C;

    A = (double*)malloc(DIM*sizeof(double));
    B = (double*)malloc(DIM*sizeof(double));
    C = (double*)malloc(DIM*sizeof(double));

    for(int i = 0; i < DIM; i++)
    {
        A[i] = 1.0;
        B[i] = 1.0;
    }
    clock_t start = clock();
    for(int j = 0; j < ITER; j++)
    {
        C[j] = 0.0;
        for(int i = 0; i < DIM; i = i+stride)
        {
            C[j] = C[j] + A[i]*B[i];
        }
    }
    clock_t end = clock();
    double T = (double)(end-start) / CLOCKS_PER_SEC;
    printf("%d, %d,%e, %e, %e\n", DIM, stride, T, Nops, Nops/T);
    
    return 0;
}