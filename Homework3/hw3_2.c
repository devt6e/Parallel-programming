#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

typedef struct Point{
    int x, y;
    int id;
}point;

int main()
{
    int myrank, ncpus;
    int lx, ly, gap, nx, ny;
    point* points;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if(myrank == 0)
    {
        printf("input (Lx, Ly, gap) : ");
        scanf("%d %d %d", &lx, &ly, &gap);
        if((lx/gap+1) * (ly/gap+1) > 10100)
        {
            printf("too many points !!\n");
            MPI_Finalize();
            return 0;
        }
    }
    MPI_Bcast(&lx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ly, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&gap, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    nx = lx / gap + 1;
    ny = ly / gap + 1;

    int localStart = nx * myrank / ncpus;
    int localEnd = nx * (myrank+1) / ncpus;

    int localNPoint = (localEnd-localStart) * ny;
    points = (point*)malloc(sizeof(point) * localNPoint);
    
    // printf("[rank %d] [%d, %d)\n",myrank, localStart, localEnd);

    for(int i = 0; i < localEnd - localStart; i++)
    {
        for(int j = 0; j < ny; j++)
        {
            int idx = j + ny*i;
            points[idx].x = (i+localStart) * gap;
            points[idx].y = j * gap;
            points[idx].id = (i+localStart)/gap + j*nx;
        }
    }

    for(int j = 0; j < ncpus; j++)
    {
        if(j != myrank)
            MPI_Barrier(MPI_COMM_WORLD);
        else
        {
            printf("[rank %d] [%d, %d)\n",myrank, localStart * gap, localEnd * gap);
            for(int i = 0; i < localNPoint; i++)
            {
                if(i%ny == 0 && i != 0)
                    printf("\n");
                if(i == localNPoint-1)
                    printf("(%d, %d)\n", points[i].x, points[i].y);
                else
                    printf("(%d, %d), ", points[i].x, points[i].y);
            }
        }
    }      
    free(points);
    MPI_Finalize();

    return 0;
}