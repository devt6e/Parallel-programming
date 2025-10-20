#include <stdio.h>
#include "mpi.h"

#define N 23

int prime(int a, int b)
{   
    //Returns the sum of prime numbers between a and b.
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
    int localStart, localEnd, localSum, total = 0;
    int ncpus, myrank;
    
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &ncpus);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // int range = N/ncpus;
    if(myrank == 0)
        localStart = 2;
    else
        localStart = N * myrank / ncpus + 1;

    localEnd = N * (myrank + 1) / ncpus;

    localSum = prime(localStart,localEnd);
    printf("[rank %d] local start %d ~ local end %d\n", myrank, localStart, localEnd);
    if(myrank != 0)
    {
        MPI_Send(&localSum, 1, MPI_INT, 0, 1001, MPI_COMM_WORLD);
    }
    else
    {
        total += localSum;
        int temp;
        for(int i = 1; i < ncpus; i++)
        {
            MPI_Recv(&temp, 1, MPI_INT, i, 1001, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += temp;
        }
        printf("[rank %d] sum of primes between 2 and %d = %d\n", myrank, N, total);
    }
    
    MPI_Finalize();
}