#include <stdio.h>
#include <mpi.h>

int main()
{
    const int ROOT = 0;
    int mrank, ncpus;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
}