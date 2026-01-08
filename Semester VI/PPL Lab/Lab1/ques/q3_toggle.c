#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {
	int rank, size;
	char string[] = "HELLO";
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank < strlen(string)) {
		if (string[rank] >= 'A' && string[rank] <= 'Z')
			string[rank] += 32;
		else if (string[rank] >= 'a' && string[rank] <= 'z')
			string[rank] -= 32;
	}
	printf("[%d] %s\n", rank, string);
	
	MPI_Finalize();
	return 0;
}
