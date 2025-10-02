#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

void Print(int* arr, int n)
{
    for(int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    srand(time(NULL));
    int ncpus, myrank;
    int seatInfo = 0;
    int nempty = 0;
    int n = 1;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    if()    
    int* arr = (int*)malloc(sizeof(int) * ncpus);
    int* emptySeat = (int*)malloc(sizeof(int) * ncpus);
    int* emptylist = (int*)malloc(sizeof(int) * ncpus);

    if(myrank == 0)
    {
        for(int i = 0; i < ncpus; i++)
            arr[i] = rand()%2;

        printf("좌석개수 : %d\n", ncpus);
        printf("좌석정보 : ");
        Print(arr, ncpus);
    }

    //MPI_Scatter(isend, 1, MPI_INT, &irecv, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(arr, 1, MPI_INT, &seatInfo, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("[rank%d] seat num : %d, seat Info : %d\n", myrank, myrank+1, seatInfo);

    MPI_Barrier(MPI_COMM_WORLD);
    
    MPI_Gather(&seatInfo, 1, MPI_INT, emptySeat, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(myrank == 0)
    {
        for(int i = 0; i < ncpus; i++)
            if(emptySeat[i] == 0)   emptylist[nempty++] = i; 
        printf("빈 자리 랭크 리스트\n");
        Print(emptylist, nempty);
    }
    if(myrank == 0)
    {
        printf("손님입장\n");
        printf("좌석정보 : ");
        Print(emptySeat, ncpus);
        if(nempty == 0)
        {
            printf("빈 자리 없음 !\n");
            return 0;
        }
        int newcus = rand()%nempty;
        int allocSeat = emptylist[newcus];
        MPI_Send(&n, 1, MPI_INT, allocSeat, 1000, MPI_COMM_WORLD);
    }

    MPI_Recv(&seatInfo, 1, MPI_INT, 0, 1000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("[rank%d] seat num : %d, seat Info : %d\n", myrank, myrank+1, seatInfo);

    MPI_Finalize();
    return 0;
}