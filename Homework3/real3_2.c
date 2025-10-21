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
    int lx, ly, gap;    //lx, ly: 각 축의 점의 개수. gap: 점을 찍는 간격
    int localStart, localEnd, localNPoint;  //각 프로세스가 생성할 영역과 점의 개수
    int npoint; //점의 개수
    point* points;  //점의 정보를 저장하는 배열

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    
    if(myrank == 0)
    {
        printf("input (lx, ly, gap): ");
        fflush(stdout);
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
            points[idx].id = (i+localStart) + j*lx + 1;
        }
    }

    for(int i = 0; i < ncpus; i++)
    {
        if(i != myrank)
            MPI_Barrier(MPI_COMM_WORLD);
        if(i == myrank )
        {
            printf("[rank %d] [%d, %d)\n",myrank, localStart * gap, localEnd * gap);
            fflush(stdout);
            for(int j = 0; j < localNPoint; j++)
            {
                if(j%ly == 0 && j != 0)
                {
                    printf("\n");
                    fflush(stdout);
                }
                if(j == localNPoint-1)
                {
                    printf("id=%d (%d, %d)\n\n", points[j].id, points[j].x, points[j].y);
                    fflush(stdout);
                }
                else
                {
                    printf("id=%d (%d, %d), ", points[j].id, points[j].x, points[j].y);
                    fflush(stdout);
                }
            }
        }
    }
    free(points);
    MPI_Finalize();

    return 0;
}