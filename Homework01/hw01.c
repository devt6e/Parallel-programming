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
    double Nacc = Nref * ITER;

    double *A, *B;

    A = (double*)malloc(DIM*sizeof(double));
    B = (double*)malloc(DIM*sizeof(double));

    for(int i = 0; i < DIM; i++)
    {
               A[i] = i;
    }
    clock_t start = clock();
    for(int j = 0; j < ITER; j++)
    {
        for(int i = 0; i < DIM; i = i+stride)
        {
            B[i] = A[i];
        }
    }
    clock_t end = clock();
    double T = (double)(end-start) / CLOCKS_PER_SEC;
    // printf("[setting] DIM = %d, ITER = %d, stride = %d\n", DIM, ITER, stride);
    // printf("[running time] %e sec\n", T);
    // printf("Nacc : %e, Tcost : %e\n", Nacc, T/Nacc);
    printf("%d, %d,%e, %e, %e\n", DIM, stride, T, Nacc, T/Nacc);
    
    return 0;
}