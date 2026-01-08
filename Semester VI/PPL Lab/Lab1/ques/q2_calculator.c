#include <mpi.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    int rank, size;
    int a = 7, b = 3;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    switch (rank % 5) {
        case 0:
			printf("[%d] Sum: %d\n", rank, a + b);
            break;
        case 1:
            printf("[%d] Difference: %d\n", rank, a - b);
            break;
        case 2:
            printf("[%d] Product: %d\n", rank, a * b);
            break;
        case 3:
            if (b != 0)
                printf("[%d] Quotient: %d\n", rank, a / b);
            else
                printf("[%d] Division by zero\n", rank);
            break;
        case 4:
            if (b != 0)
                printf("[%d] Remainder: %d\n", rank, a % b);
            else
                printf("[%d] Division by zero\n", rank);
            break;
    }

    MPI_Finalize();
    return 0;
}
