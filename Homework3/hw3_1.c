#include <stdio.h>
#include <stdlib.h>

//구조체 선언
typedef struct Point{
    int x, y;
    int id;
}point;

int main()
{
    int lx, ly, gap;    //입력 변수. gap : 점을 찍는 간격
    int nx, ny; //각 축으로의 점의 개수
    int npoint; //
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