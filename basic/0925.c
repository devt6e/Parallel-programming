//lecture note 3 page.30

#include <stdio.h>
#include "mpi.h"
#define BUF_SIZE (8500)

int main()
{
    int nrank, i;
    double a[BUF_SIZE], b[BUF_SIZE];
    
    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &nrank);

    if(nrank == 0)
    {
        MPI_Send(a, BUF_SIZE, MPI_DOUBLE, 1, 17, MPI_COMM_WORLD);
        MPI_Recv(b, BUF_SIZE, MPI_DOUBLE, 1, 19, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if(nrank == 1)
    {
        MPI_Send(a, BUF_SIZE, MPI_DOUBLE, 0, 19, MPI_COMM_WORLD);
        MPI_Recv(b, BUF_SIZE, MPI_DOUBLE, 0, 17, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    MPI_Finalize();
    return 0;
}

//9192 deadlock