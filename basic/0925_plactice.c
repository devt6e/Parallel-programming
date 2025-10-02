// 0925 0번 랭크가 1~n-1번 랭크에게 랭크 번호를 받아서 모두 합하여 출력하는 코드 작성
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main()
{
    int nrank, sum = 0;
    int ncpus;
    int x;
    MPI_Status status;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &nrank);

    // x = nrank;
    // printf("rank%d, x : %d\n", nrank, x);
    // MPI_Reduce(&x, &sum, 1, MPI_INT,MPI_SUM, 0, MPI_COMM_WORLD);
    // if(nrank == 0)
    // {
    //     printf("[rank %d] sum : %d\n",nrank, sum);
    // }
    
    if(nrank != 0)
    {
        MPI_Send(&nrank, 1, MPI_INT, 0, 1001, MPI_COMM_WORLD);
        printf("[rank %d] Send Complete\n", nrank);
    }
    else if(nrank == 0)
    {
        for(int i = 1; i < ncpus; i++)
        {
            int va;
            // printf("[rank %d] test\n", nrank);
            MPI_Recv(&va, 1, MPI_INT, i, 1001, MPI_COMM_WORLD, &status);
            printf("[rank %d] receive value : %d\n", nrank, va);
            sum += va;
        }
        printf("[rank %d] sum : %d\n",nrank, sum);
    }
    MPI_Finalize();
    return 0;
}