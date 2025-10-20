#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

enum Neighbor{LEFT, TOP,  RIGHT, BOTTOM};

typedef struct NeighborInfo{
    int x,y;
    int rank;
    int isExist;
}neinfo;

typedef struct Point{
    int x, y;
    int id;
    neinfo neighbors[4];

}point;

void GetNeighbor(int lx, int ly, int ls, int le, point* p, int myrank)
{
    int x = p->x, y = p->y;
    if(x-1 >= 0)
    {
        p->neighbors[LEFT].isExist = 1;
        p->neighbors[LEFT].x = x-1;
        p->neighbors[LEFT].y = y;
        p->neighbors[LEFT].rank = x-1<ls ? myrank-1 : myrank;
    }
    else p->neighbors[LEFT].isExist = 0;
    if(y+1 <= ly)
    {
        p->neighbors[TOP].isExist = 1;
        p->neighbors[TOP].x = x;
        p->neighbors[TOP].y = y+1;
        p->neighbors[TOP].rank = myrank;
    }
    else p->neighbors[TOP].isExist = 0;
    if(x+1 <= lx)
    {
        p->neighbors[RIGHT].isExist = 1;
        p->neighbors[RIGHT].x = x+1;
        p->neighbors[RIGHT].y = y;
        p->neighbors[RIGHT].rank = x+1>=le ? myrank+1 : myrank;
    }
    else p->neighbors[RIGHT].isExist = 0;
    if(y-1 >= 0)
    {
        p->neighbors[BOTTOM].isExist = 1;
        p->neighbors[BOTTOM].x = x;
        p->neighbors[BOTTOM].y = y-1;
        p->neighbors[BOTTOM].rank = myrank;
    }
    else p->neighbors[BOTTOM].isExist = 0;

}

int main()
{
    const char* NeiStr[] = {"LEFT", "TOP", "RIGHT", "BOTTOM"};
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
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&lx, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ly, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&gap, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    nx = lx / gap + 1;
    ny = ly / gap + 1;

    int localStart = nx * myrank / ncpus;
    int localEnd = nx * (myrank+1) / ncpus;

    int localNPoint = (localEnd-localStart) * ny;
    points = (point*)malloc(sizeof(point) * localNPoint);

    for(int i = 0; i < localEnd - localStart; i++)
    {
        for(int j = 0; j < ny; j++)
        {
            int idx = j + ny*i;
            points[idx].x = (i+localStart) * gap;
            points[idx].y = j * gap;
            points[idx].id = (i+localStart)/gap + j*nx;
            GetNeighbor(lx, ly, localStart*gap, localEnd*gap, &points[idx], myrank);
        }
    }

    for(int j = 0; j < ncpus; j++)
    {
        if(j != myrank)
            MPI_Barrier(MPI_COMM_WORLD);
        else
        {
            printf("[rank %d] x range : [%d, %d)\n",myrank, localStart * gap, localEnd * gap);
            for(int i = 0; i < localNPoint; i++)
            {
                // if(i%ny == 0 && i != 0)
                //     printf("\n");
                printf("  (%d, %d)\n ", points[i].x, points[i].y);
                for(int k = 0; k < 4; k++)
                {
                    if(points[i].neighbors[k].isExist)
                        printf("    [neighbor %s- rank%d] : (%d, %d)\n", 
                            NeiStr[k], 
                            points[i].neighbors[k].rank,
                            points[i].neighbors[k].x,
                            points[i].neighbors[k].y);
                }
            }
        }
    }      
    free(points);
    MPI_Finalize();

    return 0;
}