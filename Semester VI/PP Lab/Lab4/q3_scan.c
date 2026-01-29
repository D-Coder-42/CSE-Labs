#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int arr[4][4];
    int row[4], res[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        fprintf(stdout, "Enter 4x4 matrix: \n");
        fflush(stdout);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                scanf("%d", &arr[i][j]);
    }

    MPI_Scatter(arr, 4, MPI_INT, row, 4, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scan(row, res, 4, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        fprintf(stdout, "Output 4x4 matrix: \n");
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    fprintf(stdout, "[%d] ", rank);
    fflush(stdout);
    for (int i = 0; i < 4; i++) {
        fprintf(stdout, "%d ", res[i]);
        fflush(stdout);
    }
    fprintf(stdout, "\n");
    fflush(stdout);

    MPI_Finalize();
    return 0;
}
