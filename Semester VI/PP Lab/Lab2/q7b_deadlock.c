#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
	int rank, size;
	char string[] = "Hello World";

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    if (rank == 0) {
        fprintf(stdout, "[%d] Trying to receive\n", rank);
        fflush(stdout);
        MPI_Recv((void *)string, 256, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %s\n", rank, string);
        fflush(stdout);
        MPI_Send((void *)string, strlen(string)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

    } else {
        fprintf(stdout, "[%d] Trying to receive\n", rank);
        fflush(stdout);
        MPI_Recv((void *)string, 256, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %s\n", rank, string);
        fflush(stdout);
        MPI_Send((void *)string, strlen(string)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }

	MPI_Finalize();
	return 0;
}
