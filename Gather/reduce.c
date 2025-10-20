#include <stdio.h>
#include "mpi.h"

int main()
{
    int ncpus, myrank, ista, iend, i;
    double A[9] = {0.0,}, sum = 0.0, tsum =0.0;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    ista = 3*myrank; iend = ista + 2;
    for(int i = ista; i < iend; i++) sum += A[i];
    MPI_Reduce(&sum, &tsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(myrank == 0)
        printf("rank(%d):sum=%.2f\n",myrank,tsum);
        
    MPI_Finalize();
}