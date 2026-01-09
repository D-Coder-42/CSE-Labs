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
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	printf("[%d]", rank);
	for (int i = 50*rank+1; i <= 50*(rank+1); i++)
		if (isPrime(i)) printf (" %d", i);
	printf("\n");	
	
	MPI_Finalize();
	return 0;
}
