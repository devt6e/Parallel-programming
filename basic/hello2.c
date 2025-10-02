#include <stdio.h>
#include <mpi.h>

// int main(NULL)
int main(int argc, char* argv[])
{
    int ver, subver;
    int nRank, nProcs;
    char procName[MPI_MAX_PROCESSOR_NAME];
    int nNameLen;

    MPI_Init(NULL,NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &nRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

    MPI_Get_version(&ver, &subver);
    if(nRank == 0)
        printf("MPI Version %d.%d\n",ver, subver);
    MPI_Get_processor_name(procName, &nNameLen);

    //아래 코드대로 하면 랭크가 순서대로 나올까? -> 아닐듯?
    //1. for까지 도달하는 시간이 다름
    //2. for 루틴이 도는 시간이 다름
    //3. 틱이란게 있음?

    //그럼 뭘로 해결해?
    //앞의 프로세스한테 메세지를 받는다. -> 좋은데 너무 비효율적이지 않나요?
    //mutex 배웠어요? -> 동기화 배웠는데 왜 이야기를 안해요!!
    //뭔가 동기화 해주는 함수가 있었어 어디에 동기화를 해줄까요
    // 1. for 루틴 전, 2. if 루틴 전, 3. if 루틴 직후
    // 3.은 오류가 나지 않을까요? -> 3은 모든 프로세스가 동시에 동기화가 이뤄지지 않음.
    // 1.은 될 수도 있지만 for 루틴이 도는 시간이 다르니 보장할 수 없음.
    // 정답은 4. for루틴 마지막에 넣음
    for(int i = 0; i < nProcs; i++)
    {
        if(nRank == i)
            printf("Hello World.(Porcess name==%s, nRank==%d, nProcs==%d)\n",procName,nRank,nProcs);
        MPI_Barrier(MPI_COMM_WORLD);        
    }
    
    MPI_Finalize();
    return 0;
}