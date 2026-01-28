#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int countNonVowels(char *arr, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (isalpha(arr[i]) && !strchr("AEIOUaeiou", arr[i])) {
            count++;
        }
    }
    return count;
}

int main (int argc, char** argv) {
    int rank, size, M;
    int count, total;
    char *arr, *part;
    int *countArr;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        char buffer[1024];
        fprintf(stdout, "Enter string: ");
        fflush(stdout);
        fgets(buffer, sizeof(buffer), stdin);
        M = strlen(buffer) / size;
        arr = (char*)malloc(M * size * sizeof(char));
        strcpy(arr, buffer);
        countArr = (int*)malloc(size * sizeof(int));
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    part = (char*)malloc(M * sizeof(char));

    MPI_Scatter(arr, M, MPI_CHAR, part, M, MPI_CHAR, 0, MPI_COMM_WORLD);
    fprintf(stdout, "[%d] Received: ", rank);
    fflush(stdout);
    for (int i = 0; i < M; i++) {
        fprintf(stdout, "%c", part[i]);
        fflush(stdout);
    }
    fprintf(stdout, "\n");
    fflush(stdout);

    count = countNonVowels(part, M);
    MPI_Gather(&count, 1, MPI_INT, countArr, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        total = 0;
        for (int i = 0; i < size; i++) {
            fprintf(stdout, "[%d] Non-vowel count for process %d: %d\n", rank, i, countArr[i]);
            fflush(stdout);
            total += countArr[i];
        }
        fprintf(stdout, "[%d] Total non-vowel count is: %d\n", rank, total);
        fflush(stdout);

        free(arr);
        free(countArr);
    }

    free(part);
    MPI_Finalize();
    return 0;
}