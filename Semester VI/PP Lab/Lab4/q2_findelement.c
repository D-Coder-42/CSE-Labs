#include <mpi.h>
#include <stdio.h>

int countInstance(int *arr, int n, int key) {
    int count = 0;
    for (int i = 0; i < n; i++) 
        if (arr[i] == key) count++;
    return count;
}

int main(int argc, char *argv[])
{
    int rank, size;
    int count, ans, key;
    int arr[3][3];
    int row[3];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        fprintf(stdout, "Enter 3x3 matrix: \n");
        fflush(stdout);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                scanf("%d", &arr[i][j]);
        fprintf(stdout, "Enter element to find: ");
        fflush(stdout);
        scanf("%d", &key);
    }

    MPI_Bcast(&key, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(arr, 3, MPI_INT, row, 3, MPI_INT, 0, MPI_COMM_WORLD);
    count = countInstance(row, 3, key);
    MPI_Reduce(&count, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fprintf(stdout, "[%d] Count of instances of %d: %d\n", rank, key, ans);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
