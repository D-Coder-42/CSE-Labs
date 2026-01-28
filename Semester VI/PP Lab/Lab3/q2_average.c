#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

float average(int *arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return (float)sum / n;
}

int main (int argc, char** argv) {
    int rank, size, M;
    float avg, ans;
    int *arr, *part;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        fprintf(stdout, "Enter value of 'M': ");
        fflush(stdout);
        scanf("%d", &M);
        arr = (int*)malloc(M * size * sizeof(int));
        for (int i = 0; i < M*size; i++) {
            fprintf(stdout, "Enter number %d: ", i+1);
            fflush(stdout);
            scanf("%d", &arr[i]);
        }
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    part = (int*)malloc(M * sizeof(int));

    MPI_Scatter(arr, M, MPI_INT, part, M, MPI_INT, 0, MPI_COMM_WORLD);
    fprintf(stdout, "[%d] Received: ", rank);
    fflush(stdout);
    for (int i = 0; i < M; i++) {
        fprintf(stdout, "%d ", part[i]);
        fflush(stdout);
    }
    fprintf(stdout, "\n");
    fflush(stdout);

    avg = average(part, M);
    MPI_Reduce(&avg, &ans, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ans = ans / size;
        fprintf(stdout, "[%d] The average is: %.3f\n", rank, ans);
        fflush(stdout);
    }

    free(part);
    if (rank == 0) free(arr);
    MPI_Finalize();
    return 0;
}