#include <stdio.h>
#include "mpi.h"

#define N 31

int prime(int a, int b)
{   
    int sum = 0;

    for (int i = a; i <= b; i++)
    {
        if (i < 2) continue; // 0과 1은 소수가 아님

        int isPrime = 1; // 소수라고 가정
        for (int j = 2; j * j <= i; j++)
        {
            if (i % j == 0)
            {
                isPrime = 0;
                break;
            }
        }

        if (isPrime)
            sum += i;
    }

    return sum;
}

int main()
{
    int myprimeSum = 0, preprimeSum = 0, myprime = 0, result = 0;
    int ncpus, myrank;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    myprimeSum = prime(1, myrank + 1);
    if(myrank != N-1)
    {
        MPI_Send(&myprimeSum, 1, MPI_INT, myrank+1, 1002, MPI_COMM_WORLD);
        printf("[rank %d] send myprimeSum %d\n", myrank, myprimeSum);
    }
    
    if(myrank != 0)
    {
        MPI_Recv(&preprimeSum, 1, MPI_INT, myrank-1, 1002, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        myprime = myprimeSum - preprimeSum;    
    }

    if(myrank != 0)
        MPI_Send(&myprime, 1, MPI_INT, 0, 1001, MPI_COMM_WORLD);
    
    if(myrank == 0)
    {
        int temp = 0;
        for(int i = 1; i < N; i++)
        {
            MPI_Recv(&temp, 1, MPI_INT, i, 1001, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            result += temp;
        }
        printf("[rank %d] sum of prime numbers between 1 and N : %d\n", myrank, result);
    }
    
    MPI_Finalize();
}


    // int sum = 0;

    // for (int i = a; i <= b; i++)
    // {
    //     if (i < 2) continue; // 0과 1은 소수가 아님

    //     int isPrime = 1; // 소수라고 가정
    //     for (int j = 2; j * j <= i; j++)
    //     {
    //         if (i % j == 0)
    //         {
    //             isPrime = 0;
    //             break;
    //         }
    //     }

    //     if (isPrime)
    //         sum += i;
    // }

    // return sum;