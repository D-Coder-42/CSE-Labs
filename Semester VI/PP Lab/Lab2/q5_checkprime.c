#include <mpi.h>
#include <stdio.h>

int isPrime (int n) {
    if (n <= 1) return 0;
    for (int i = 2; i < n; i++)
        if (n % i == 0) return 0;
    return 1;
}

int main (int argc, char *argv[]) {
	int rank, size;
	int x, arr[256];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            fprintf(stdout, "Enter number %d: ", i+1);
            fflush(stdout);
            scanf("%d", &arr[i]);
        }
        for (int i = 1; i < size; i++)
            MPI_Send(&arr[i], 1, MPI_INT, i, i, MPI_COMM_WORLD);
        x = arr[rank];
    } else {
        MPI_Recv(&x, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
    }

    fprintf(stdout, "[%d] %d %s\n", rank, x, isPrime(x) ? "is prime" : "is not prime");
    fflush(stdout);

	MPI_Finalize();
	return 0;
}
