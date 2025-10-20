#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;

    // MPI 환경 초기화 (가장 중요!)
    MPI_Init(&argc, &argv);

    // 이 시점 이후에 MPI_Comm_size 호출 가능
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("총 프로세스 수: %d\n", size);
    }

    MPI_Finalize();
    return 0;
}