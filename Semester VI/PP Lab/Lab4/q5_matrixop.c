#include <mpi.h>
#include <stdio.h>

int maxCol(int* col, int n) {
    int max = col[0];
    for (int i = 1; i < n; i++)
        if (col[i] > max) max = col[i];
    return max;
}
int minCol(int* col, int n) {
    int min = col[0];
    for (int i = 1; i < n; i++)
        if (col[i] < min) min = col[i];
    return min;
}

void editRow(int* row, int* col, int n, int rank) {
    for (int i = 0; i < n; i++) {
        if (i < rank) row[i] = maxCol(col, n);
        else if (i > rank) row[i] = minCol(col,n);
        else row[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    int arrA[5][5], arrB[5][5];
    int row[5], col[5];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        fprintf(stdout, "Enter 5x5 matrix: \n");
        fflush(stdout);
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                scanf("%d", &arrA[i][j]);
    }

    MPI_Scatter(arrA, 5, MPI_INT, row, 5, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Alltoall(row, 1, MPI_INT, col, 1, MPI_INT, MPI_COMM_WORLD);
    editRow(row, col, 5, rank);
    MPI_Gather(row, 5, MPI_INT, arrB, 5, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fprintf(stdout, "Output 5x5 matrix: \n");
        fflush(stdout);
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                fprintf(stdout, "%d\t", arrB[i][j]);
                fflush(stdout);
            }
            fprintf(stdout, "\n");
            fflush(stdout);
        }
    }

    MPI_Finalize();
    return 0;
}
