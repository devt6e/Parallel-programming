#include <stdio.h>
#include <stdlib.h>

typedef struct Point{
    int x, y;
    int id;
}point;

int main()
{
    int lx, ly, gap;
    int nx, ny;
    int npoint;
    point* points;
    
    printf("Input (lx, ly, gap) : ");
    scanf("%d %d %d", &lx, &ly, &gap);

    nx = lx/gap + 1;
    ny = ly/gap + 1;
    npoint = nx * ny;
    if(npoint > 10100)
    {
        printf("too many points !!\n");
        return 0;
    }
    points = (point*)malloc(sizeof(point)*npoint);


    for(int i = 0; i < nx; i++)
    {
        for(int j = 0; j < ny; j++)
        {
            int idx = j + ny*i;
            points[idx].x = i * gap;
            points[idx].y = j * gap;
            points[idx].id = idx + 1;
        }
    }

    for(int i = 0; i < npoint; i++)
    {
        printf("[point %d]-(%d, %d)\n", points[i].id, points[i].x, points[i].y);
    }
    return 0;
}