#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BDX 17
#define BDY 10

#define COND_EMPTY 0

typedef struct Block
{
    int x, y;
    int value;
}block_t;

//보드 초기화 시 짝이 되는 숫자 생성. 랜덤(가로 또는 세로, )
void GeneratePair(int** BD, int targetX, int targetY, int targetNum)
{
    int isRow;  //가로, 세로 결정 변수(랜덤) 1이면 세로 랜덤 위치에 짝 생성
    isRow = rand()%2;
    if(isRow)
    {
        for(int i = 0; i < BDY; i++)
        {
            if(rand()%2 == 1 && BD[targetX][i] == COND_EMPTY)
            {
                BD[targetX][i] = 10 - targetNum;
            }
        }
    }
}

void FindEmpty(int** BD, int target)
{

}

void Init(int** BD)
{
    for(int i = 0; i < BDX; i++)
    {
        for(int j = 0; j < BDY; j++)
        {
            if(BD[i][j] == COND_EMPTY)
            {
                int isRow = rand()%2;
                if(isRow)
                {

                }
            }
        }
    }
}

int main()
{
    int board[BDX][BDY];


}