#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main()
{
    int myrank, ncpus;
    int myseat;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    srand(time(NULL) + myrank);

    myseat = rand()%2;
}
