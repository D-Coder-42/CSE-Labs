#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int process(int n, int rank) {
    if (rank % 2 == 0) return n*n;
    return n*n*n;
}

int main (int argc, char *argv[]) {
	int rank, size;
	int x, arr[256];

    int bufsize = 256 * (MPI_BSEND_OVERHEAD + sizeof(MPI_INT));
    void* buf = (void*) malloc(bufsize);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    MPI_Buffer_attach(buf, bufsize);
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            fprintf(stdout, "Enter number %d: ", i);
            fflush(stdout);
            scanf("%d", &arr[i-1]);
        }
        for (int i = 1; i < size; i++)
            MPI_Bsend(&arr[i-1], 1, MPI_INT, i, i, MPI_COMM_WORLD);

    } else {
        MPI_Recv(&x, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Processed element: %d\n", rank, process(x,rank));
        fflush(stdout);
    }
    MPI_Buffer_detach(buf, &bufsize);
	MPI_Finalize();
	return 0;
}
