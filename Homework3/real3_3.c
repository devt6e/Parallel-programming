#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

enum NEIGHBOR{LEFT, UP, RIGHT, DOWN};

typedef struct Neighbor{
    int x, y;
    int rank;
    int valid;
}neighbor;

typedef struct Point{
    int x, y;
    int id;
    neighbor neighbors[4];
}point;

void GetNeighbor(point* p, int Lx, int Ly)
{
    int x = p->x;
    int y = p->y;
    for(int i = 0; i < 4; i++)
    {
        p->neighbors[i].x = x; 
        p->neighbors[i].y = y; 
        p->neighbors[i].valid = 0;
    }

    if(x-1 >= 0)
    {
        p->neighbors[LEFT].x--;
        p->neighbors[LEFT].valid = 1;
    }
    if(x+1 < Lx)
    {
        p->neighbors[RIGHT].x++;
        p->neighbors[RIGHT].valid = 1;
    }
    if(y-1 >= 0)
    {
        p->neighbors[DOWN].y--;
        p->neighbors[DOWN].valid = 1;
    }
    if(y+1 < Ly)
    {
        p->neighbors[UP].y++;
        p->neighbors[UP].valid = 1;
    }
}

int main()
{
    const char* NeiStr[] = {"LEFT", "UP", "RIGHT", "DOWN"};
    const int START = 0, END = 1;
    int myrank, ncpus;
    int lx, ly, gap, Lx, Ly;
    int npoint;
    int localStart, localEnd, localNPoint;
    int* myRange[2];
    point* points;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);

    for(int i = 0; i < 2 ; i++)
        myRange[i] = (int*)malloc(sizeof(int) * ncpus);
    
    if(myrank == 0)
    {
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

    Lx = (lx-1) * gap; 
    Ly = (ly-1) * gap;

    localStart  = lx * myrank / ncpus;
    localEnd = lx * (myrank+1) / ncpus;
    localNPoint = (localEnd- localStart)*ly; 

    MPI_Allgather(&localStart, 1, MPI_INT, myRange[START], 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(&localEnd, 1, MPI_INT, myRange[END], 1, MPI_INT, MPI_COMM_WORLD);

    points = (point*)malloc(sizeof(point)*localNPoint);

    for(int i = 0; i < localEnd - localStart; i++)
    {
        for(int j = 0; j < ly; j++)
        {
            int idx = j + i*ly;
            points[idx].x = (i+localStart) * gap;
            points[idx].y = j * gap;
            points[idx].id = (i+localStart) + j*lx;

            GetNeighbor(points+idx, Lx, Ly);
            for(int k = 0; k < 4; k++)
            {
                if(points[idx].neighbors[k].valid)
                {
                    int x = points[idx].neighbors[k].x;
                    for(int l = 0; l < ncpus; l++)
                    {
                        if( myRange[START][l] * gap <= x && x < myRange[END][l] * gap)
                            points[idx].neighbors[k].rank = l;
                    }
                }
            }
        }
    }

    for(int i = 0; i < ncpus; i++)
    {
        if(i == myrank)
        {
            printf("[rank %d] (%d <= x < %d)\n",myrank, localStart * gap, localEnd * gap);
            for(int j = 0; j < localNPoint; j++)
            {
                printf("id=%d (%d, %d)\n", points[j].id, points[j].x, points[j].y);
                for(int k = 0; k < 4; k++)
                {
                    if(points[j].neighbors[k].valid)
                        printf("    [neighbor;%s] (%d, %d)->rank%d\n", 
                            NeiStr[k], 
                            points[j].neighbors[k].x,
                            points[j].neighbors[k].y,
                            points[j].neighbors[k].rank);
                }
            }
            printf("\n"); 
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    for(int i = 0; i < 2; i++)
    {
        free(myRange[i]);
    }
    free(points);
    MPI_Finalize();

    return 0;
}