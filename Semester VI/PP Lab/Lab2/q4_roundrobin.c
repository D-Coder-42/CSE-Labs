#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
	int rank, size;
	int x;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    if (rank == 0) {
        fprintf(stdout, "Enter number: ");
        fflush(stdout);
        scanf("%d", &x);
        x++;
    } else {
        MPI_Recv(&x, 1, MPI_INT, (rank-1+size)%size, (rank-1+size)%size, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %d\n", rank, x);
        fflush(stdout);
        x++;
    }

    MPI_Send(&x, 1, MPI_INT, (rank+1)%size, rank, MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Recv(&x, 1, MPI_INT, (rank-1+size)%size, (rank-1+size)%size, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %d\n", rank, x);
        fflush(stdout);
    }
    
	MPI_Finalize();
	return 0;
}
