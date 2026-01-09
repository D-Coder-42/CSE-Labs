#include <mpi.h>
#include <stdio.h>

int fact (int n) {
	int res = 1;
	for (int i = 1; i <= n; i++)
		res *= i;
	return res;
}

int fib (int n) {
	if (n <= 1) return n;
	return fib(n-1) + fib(n-2); 
}

int main (int argc, char *argv[]) {
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	switch (rank % 2) {
		case 0:
			printf("[%d] Factorial of %d: %d\n", rank, rank, fact(rank));
			break;
		case 1:
			printf("[%d] Fibonacci number #%d: %d\n", rank, rank, fib(rank));
			break;
	}
	
	MPI_Finalize();
	return 0;
}
