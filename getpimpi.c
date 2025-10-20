#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

#define N 1000000000 //10억

int main()
{
    
    static int root = 0;
    int myrank, ncpus;
    double x, y;
    double stime, etime, ttime;
    long long mycnt = 0;
    long long totalcnt = 0;
    long long buf;

    MPI_Init(NULL, NULL);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);

    if(myrank == root)
        printf("N : %ld, ncpus : %d\n", N, ncpus);
    stime = MPI_Wtime();
    for(int i = 0; i < N/ncpus; i++)
    {
        x = (double)rand()/RAND_MAX;
        y = (double)rand()/RAND_MAX;
        if(x*x+y*y <= 1) mycnt++;
    }

    if(myrank != root)
        MPI_Send(&mycnt, 1, MPI_INT, 0, 1000, MPI_COMM_WORLD);
    
    if(myrank == root)
    {
        totalcnt += mycnt;
        for(int i = 1; i < ncpus; i++)
        {
            MPI_Recv(&buf, 1, MPI_INT, i, 1000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            totalcnt += buf;
        }
        etime = MPI_Wtime();
        ttime = etime - stime;
        printf("%.10f / run time : %f\n", 4*totalcnt / (double)N, ttime);
    }
    MPI_Finalize();
    return 0;
}