#include <stdio.h>
#include <stdlib.h>

typedef struct Point{
    int x, y;
    int status;
    int id;
}point;

int main()
{
    int lx, ly, gap;
    int nx, ny;
    int npoint;
    point** points;
    
    printf("Input (lx, ly, gap) : ");
    scanf("%d %d %d", &lx, &ly, &gap);

    points = (point**)malloc(sizeof(point)*lx);
    for(int i = 0; i < lx; i++)
    {
        points[i] = (point*)malloc(sizeof(point)*ly);
    }
    for(int i = 0; i < lx; i++)
    {
        for(int j = 0; j < ly; j++)
        {
            points[i][j].status = 0;
            if(i%gap == 0 && j%gap == 0)
            {
                points[i][j].status = 1;
                points[i][j].x = i;
                points[i][j].y = j;
                points[i][j].id = (i/gap) * (lx/gap + 1) + 1;
                printf("[id %d] : (%d, %d) ",points[i][j].id, points[i][j].x, points[i][j].y);
            }
        }
        printf("\n");
    }

    for(int i = 0; i < lx; i++)
        free(points[i]);
    free(points);
    return 0;
}