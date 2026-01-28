#include <mpi.h>
#include <stdio.h>

int fact(int n) {
    if (n <= 1) return 1;
    else return n * fact(n - 1);
}

int main (int argc, char** argv) {
    int rank, size;
    int x, sum;
    int arr[16];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            fprintf(stdout, "Enter number %d: ", i+1);
            fflush(stdout);
            scanf("%d", &arr[i]);
        }
    }

    MPI_Scatter(arr, 1, MPI_INT, &x, 1, MPI_INT, 0, MPI_COMM_WORLD);
    fprintf(stdout, "[%d] Received: %d\n", rank, x);
    fflush(stdout);

    x = fact(x);
    MPI_Reduce(&x, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fprintf(stdout, "[%d] The sum of factorials is: %d\n", rank, sum);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}