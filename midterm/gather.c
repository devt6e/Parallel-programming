#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main()
{
    int myrank, ncpus;
    int mynum, temp;
    int* arr, *arr2;
    MPI_Init(NULL,NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    mynum = myrank * 10;
    arr = (int*)malloc(sizeof(int)*ncpus);
    arr2 = (int*)malloc(sizeof(int)*ncpus);

    if(myrank != 0)
        MPI_Send(&mynum, 1, MPI_INT, 0, 1001, MPI_COMM_WORLD);
    if(myrank == 0)
    {
        arr[0] = mynum;
        for(int i = 1; i < ncpus; i++)
        {
            MPI_Recv(&temp, 1, MPI_INT, i, 1001, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            arr[i] = temp;
        }
        for(int i = 0; i < ncpus; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }
    MPI_Gather(&mynum,1,MPI_INT,arr2,1,MPI_INT,0,MPI_COMM_WORLD);
    if(myrank == 0)
    {
        for(int i = 0; i < ncpus; i++)
            printf("%d ", arr2[i]);
        printf("\n");
    }
    free(arr);
    free(arr2);
    MPI_Finalize();
    
    return 0;
}