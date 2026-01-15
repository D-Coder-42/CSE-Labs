#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
	int rank, size;
	int x, arr[256];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            fprintf(stdout, "Enter number %d: ", i);
            fflush(stdout);
            scanf("%d", &arr[i-1]);
        }
        for (int i = 1; i < size; i++)
            MPI_Send(&arr[i-1], 1, MPI_INT, i, i, MPI_COMM_WORLD);

    } else {
        MPI_Recv(&x, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %d\n", rank, x);
        fflush(stdout);
    }

	MPI_Finalize();
	return 0;
}
