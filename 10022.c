#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mpi.h"

int main()
{
    int ncpus, myrank;
    int myseat, nempty = 0;
    int* seats;
    int* empties;
    static int root = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    srand(time(NULL) + myrank);
    myseat = rand()%2;
    if(myrank == root)
    {
        seats = (int*)malloc(sizeof(int)*ncpus);
        empties = (int*)malloc(sizeof(int)*ncpus);
    }
    // printf("1 [rank %d] seat info : %d\n", myrank, myseat);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Gather(&myseat, 1, MPI_INT, seats, 1,MPI_INT, root, MPI_COMM_WORLD);
    if(myrank == root)
    {
        printf("seats : ");
        for(int i = 0; i < ncpus; i++)
            printf("%d ", seats[i]);
        printf("\n");
    }
    // sleep(1);
    MPI_Barrier(MPI_COMM_WORLD);

    if(myrank == root)
    {
        for(int i = 0; i < ncpus; i++)
            if(seats[i] == 0) empties[nempty++] = i;
        
        // int ncstm = rand()%nempty;
        //printf("customer num : %d\n", ncstm);
        if(nempty == 0)
        {
            printf("빈 자리 없음 !\n");
        }
        else
        {
            int newcstm = empties[rand()%nempty];
            // printf("new customer %d's seat %d\n", i+1, newcstm);
            printf("new customer's seat %d\n", newcstm);
            seats[newcstm] = 1;
        }

    }
    MPI_Scatter(seats, 1, MPI_INT, &myseat, 1, MPI_INT, root, MPI_COMM_WORLD);

    if(myrank == root)
    {
        printf("seats : ");
        for(int i = 0; i < ncpus; i++)
            printf("%d ", seats[i]);
        printf("\n");
    }

    if(myrank == root)
    {
        free(seats);
        free(empties);
    }
    MPI_Finalize();
    return 0;
}