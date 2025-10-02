#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NSIZE 10000000

int main(int argc, char* argv[])
{
    int x;
    double *src, *dest;
    double elapsed_time;

    MPI_Status status;
    MPI_Init(NULL, NULL);

    //get number of processes
    int ncpus;
    int local_size;
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);

    //get the rank of the process
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    local_size = NSIZE/ncpus;

    src = malloc(sizeof(double)*local_size);
    for(int i = 0; i < local_size; i++)
    {
        src[i] = myrank;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    // printf("myrank : %d, src : ", myrank);
    // for(int i = 0; i < local_size; i++)
    //     printf("%f ", src[i]);
    // printf("\n");

    dest = malloc(sizeof(double)*local_size);

    // elapsed_time = -MPI_Wtime();    
    // //단순한 복사
    // for(int i = 0; i < local_size; i++)
    // {
    //     dest[i] = src[i];
    // }
    // elapsed_time += MPI_Wtime();

    //printf("myrank = %d, time = %f (sec) \n", myrank, elapsed_time);
    //MPI_Wtime() : 현재 cpu의 시간을 받아오는 함수.

    //MPI_Send(data, count, datatype, rank, tag, communicator,)
    //MPI_Recv(data, count, datatype, rank, tag, communicator, status)

    //try 1
    // MPI_Send(src, local_size, double*, (myrank+1)%ncpus, myrank, MPI_COMM_WORLD);
    // MPI_Recv(dest, local_size, double*, (myrank-1)%ncpus, (myrank-1)%ncpus, MPI_COMM_WORLD, &status);

    //try2
    // elapsed_time = -MPI_Wtime();    
    // for(int i = 0; i < ncpus; i++)
    // {
    //     if(myrank == i)
    //     {
    //         //printf("rank %d send start\n", myrank);
    //         MPI_Send(src, local_size, MPI_DOUBLE, (myrank+1)%ncpus, 1000, MPI_COMM_WORLD);
    //         //printf("rank %d send complete\n\n", myrank);
    //     }
        
    // }
    // MPI_Barrier(MPI_COMM_WORLD);
    // for(int i = 0; i < ncpus; i++)
    // {
    //     if(myrank == i)
    //     {
    //         //printf("rank %d receive start\n", myrank);
    //         MPI_Recv(dest, local_size, MPI_DOUBLE, (myrank+ncpus-1)%ncpus, 1000, MPI_COMM_WORLD, &status);
    //         //printf("rank %d receive complete\n\n", myrank);
    //     }
        
    // }
    // elapsed_time += MPI_Wtime();
    //printf("myrank : %d, dest[0] : %f, time : %12.6e(sec)\n", myrank, dest[0], elapsed_time);

    //try 3(Ryu)
    elapsed_time = -MPI_Wtime();    
    if(myrank == 0)
    {
        MPI_Send(src, local_size, MPI_DOUBLE, 1, 1000, MPI_COMM_WORLD);
        MPI_Recv(dest, local_size, MPI_DOUBLE, ncpus-1, 1000, MPI_COMM_WORLD, &status);
    }
    else if(myrank == ncpus-1)
    {
        MPI_Send(src, local_size, MPI_DOUBLE, 0, 1000, MPI_COMM_WORLD);
        MPI_Recv(dest, local_size, MPI_DOUBLE, myrank-1, 1000, MPI_COMM_WORLD, &status);
    }
    else
    {
        MPI_Send(src, local_size, MPI_DOUBLE, myrank+1, 1000, MPI_COMM_WORLD);
        MPI_Recv(dest, local_size, MPI_DOUBLE, myrank-1, 1000, MPI_COMM_WORLD, &status);
    }
    elapsed_time += MPI_Wtime();
    printf("myrank : %d, dest[0] : %f, time : %12.6e(sec)\n", myrank, dest[0], elapsed_time);

    //Q. MPI_finalize와 메모리 해제 중 무엇이 선행되어야 할까?
    //A. 메모리 해제가 선행되어야 한다.
    //프로세스끼리는 서로 바라보지 못한다. 프롤세스가 종료된다면 어떤 일이 발생할지 모른다.
    //가급적이면 해제를 먼저하자.
    free(src);
    free(dest);

    MPI_Finalize();
}