#include <stdio.h>
#include <mpi.h>

int main()
{
    int myrank, ncpus, ROOT = 0;
    int isend, irecv[4] = {0,}, recvdata[4] = {0, };
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // MPI_Request req[4];

    isend = myrank + 1;
    printf("[rank %d] isend : %d\n", myrank, isend);

    if(myrank != ROOT)
    {
        for(int i = 1; i < ncpus; i++)
        {
            //MPI_Isend(&isend, 1, MPI_INT, ROOT, 1001, MPI_COMM_WORLD, req+i);
            MPI_Send(&isend, 1, MPI_INT, ROOT, 1001, MPI_COMM_WORLD);
        }
        // MPI_Waitall(4, req, MPI_STATUS_IGNORE);
    }
    
    if(myrank == ROOT)
    {
        irecv[0] = isend;
        for(int i = 1; i < ncpus; i++)
        {
            int va;
            MPI_Recv(&va, 1, MPI_INT, i, 1001, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            irecv[i] = va;
        }
    }

    if(myrank == ROOT)
    {
        printf("irecv : ");
        for(int i = 0; i < ncpus; i++)
        {
            printf("%d ", irecv[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}