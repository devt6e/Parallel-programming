#include <stdio.h>
#include <mpi.h>

int main()
{
    int ncpus, myrank;
    int x = 0;
    MPI_Status status;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank); 

    if(ncpus < 2)
    {
        printf("please use +2 procs.\n");
        MPI_Finalize();
        return 0;
    }
    /*
        MPI_Send(&x, 1, MPI_INT, 1, 1002, MPI_COMM_WORLD);
        MPI_Recv(&x, 1, MPI_INT, 1, 1002, MPI_COMM_WORLD, &status);

        이렇게 해버리면 deadlock 발생
        -모든 프로세스가 1번으로 보냄.
        -모든 프로세스가 1번에게 받음.
        send-recv pair가 맞지않음.
        반드시 조건문이 필요함

    */
    x = myrank;
    if(myrank == 0)
    {
        x = 10;
        MPI_Send(&x, 1, MPI_INT, 1, 1002, MPI_COMM_WORLD);
        printf("RANK 1 Send x : %d\n", x);
    }
    else if(myrank == 1)
    {
        MPI_Recv(&x, 1, MPI_INT, 0, 1002, MPI_COMM_WORLD, &status);
        printf("RANK 1 Recieved x : %d\n", x);
    }
    MPI_Finalize();

    return 0;
}