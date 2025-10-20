#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

typedef struct Point{
    int x, y;
    int id;
}point;

int main()
{
    int myrank, ncpus;
    int lx, ly, gap;
    int localStart, localEnd, localNPoint;
    int npoint;
    point* points;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    
    // sleep(1);
    if(myrank == 0)
    {
        // printf("input (lx, ly, gap) :");
        scanf("%d %d %d", &lx, &ly, &gap);
        npoint = lx * ly;
        if(npoint > 10100)
        {
            printf("too many point(Max 10100)\n");
            MPI_Finalize();
            return 0;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&lx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ly, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&gap, 1, MPI_INT, 0, MPI_COMM_WORLD);

    localStart = lx * myrank / ncpus;
    localEnd = lx * (myrank+1) / ncpus;
    localNPoint = (localEnd-localStart)*ly; 

    points = (point*)malloc(sizeof(point)*localNPoint);

    for(int i = 0; i < localEnd - localStart; i++)
    {
        for(int j = 0; j < ly; j++)
        {
            int idx = j + i*ly;
            points[idx].x = (i+localStart) * gap;
            points[idx].y = j * gap;
            points[idx].id = i + localStart + j*lx;
        }
    }

    for(int i = 0; i < ncpus; i++)
    {
        // if(i!=myrank)
            // MPI_Barrier(MPI_COMM_WORLD);
        // else
        if( i == myrank )
        {
            printf("[rank %d] [%d, %d)\n",myrank, localStart * gap, localEnd * gap);
            for(int j = 0; j < localNPoint; j++)
            {
                if((j%ly == 0 || j%5 == 0) && j != 0)
                    printf("\n");
                if(j == localNPoint-1)
                    printf("id=%d (%d, %d)\n\n", points[j].id, points[j].x, points[j].y);
                else
                    printf("id=%d (%d, %d), ", points[j].id, points[j].x, points[j].y);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    free(points);
    MPI_Finalize();

    return 0;
}