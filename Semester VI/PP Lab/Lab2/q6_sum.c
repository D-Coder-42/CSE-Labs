#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int fact (int n) {
	int res = 1;
	for (int i = 1; i <= n; i++)
		res *= i;
	return res;
}

int process(int rank) {
    if (rank % 2 == 0) return fact(rank+1);
    return (rank+1)*(rank+2)/2;
}

int main (int argc, char *argv[]) {
	int rank, size;
	int x, res = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    x = process(rank);
    if (rank == 0) res += x;

    if (rank != 0)
        MPI_Send(&x, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
    else
        for (int i = 1; i < size; i++) {
            MPI_Recv(&x, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
            res += x;
        }

    if (rank == 0) {
        fprintf(stdout, "[%d] Result: %d\n", rank, res);
        fflush(stdout);
    }

	MPI_Finalize();
	return 0;
}
