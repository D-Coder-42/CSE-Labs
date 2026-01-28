#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void exponent(int* arr, int n, int rank) {
    for (int i = 0; i < n; i++)
        arr[i] = pow(arr[i], rank+2);
}

int main (int argc, char** argv) {
    int rank, size;
    int *arr, *part, M;
    int *ans;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        fprintf(stdout, "Enter value of M: ");
        fflush(stdout);
        scanf("%d", &M);
        arr = (int*)malloc(M * size * sizeof(int));
        fprintf(stdout, "Enter %d numbers: ", M * size);
        fflush(stdout);
        for (int i = 0; i < M * size; i++) {
            scanf("%d", &arr[i]);
        }
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    part = (int*)malloc(M * sizeof(int));
    MPI_Scatter(arr, M, MPI_INT, part, M, MPI_INT, 0, MPI_COMM_WORLD);
    exponent(part, M, rank);
    MPI_Gather(part, M, MPI_INT, arr, M, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fprintf(stdout, "[%d] Edited array: ", rank);
        fflush(stdout);
        for (int i = 0; i < M *size; i++) {
            fprintf(stdout, "%d ", arr[i]);
            fflush(stdout);
        }
        fprintf(stdout, "\n");
        fflush(stdout);

        free(arr);
    }

    free(part);
    MPI_Finalize();
    return 0;
}