#include <stdio.h>
#include <stdlib.h>

typedef struct Point{
    int x,y;
    int id;
}point;

int main()
{
    int lx, ly, gap;    //lx, ly: 각 축의 점의 개수. gap: 점을 찍는 간격
    int npoint; //점의 개수
    point* points;  //점들을 저장할 배열

    printf("input (lx, ly, gap) : ");
    scanf("%d %d %d", &lx, &ly, &gap);
    npoint = lx * ly;
    if(npoint > 10100)
    {
        printf("점 개수 초과. (Max:10100)\n");
        return 0;
    }
    points =(point*)malloc(sizeof(point)*npoint);
    
    for(int i = 0; i < lx; i++)
    {
        for(int j = 0; j < ly; j++)
        {
            int idx = j + ly*i;
            points[idx].x = i * gap;
            points[idx].y = j * gap;
            points[idx].id = idx + 1;
        }
    }

    for(int i = 0; i < npoint; i++)
        printf("[id %d]-(%d, %d)\n", points[i].id, points[i].x, points[i].y);

    free(points);
    return 0;
}