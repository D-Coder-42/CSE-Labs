#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main (int argc, char *argv[]) {
	int rank, size;
	int x = 7;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Part A (pow)
	printf("[%d] %d ^ %d = %.0f\n", rank, x, rank, pow(x, rank));
	// Part B (Hello/World)
	printf(rank%2 == 0 ? "[%d] Hello\n" : "[%d] World\n", rank);

	MPI_Finalize();
	return 0;
}
