#include <stdio.h>
#include "mpi.h"

int main()
{
    int i, myrank;
    int isend[3] = {0 ,}, irecv = 0;
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if(myrank == 0) for(i = 0; i < 3; i++) isend[i] = i+1;

    MPI_Scatter(isend, 1, MPI_INT, &irecv, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("myrank : %d, irecv=%d\n", myrank, irecv);

    if(myrank == 0)
    {
        for(i = 0; i < 3; i++) isend[i] = (i+1)*10;
    }
    else
    {
        for(i = 9; i < 3; i++) isend[i] = 0;
    }
    MPI_Finalize();

    return 0;

}