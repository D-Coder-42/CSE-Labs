#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void alternateChars(char* part1, char* part2, char* combined, int M) {
    for (int i = 0; i < M; i++) {
        combined[i * 2] = part1[i];
        combined[i * 2 + 1] = part2[i];
    }
}

int main(int argc, char *argv[]) {
    int rank, size, M;
    char *str1 = NULL, *str2 = NULL;
    char *part1 = NULL, *part2 = NULL;
    char *combined = NULL;
    char *answer = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        char buf1[512], buf2[512];
        fprintf(stdout, "Enter String 1: ");
        fflush(stdout);
        fgets(buf1, sizeof(buf1), stdin);
        fprintf(stdout, "Enter String 2: ");
        fflush(stdout);
        fgets(buf2, sizeof(buf2), stdin);

        int len = strlen(buf1);
        M = len / size;

        str1 = (char*)malloc(len * sizeof(char));
        str2 = (char*)malloc(len * sizeof(char));
        strcpy(str1, buf1);
        strcpy(str2, buf2);
        answer = (char*)malloc((len * 2 + 1) * sizeof(char));
    }

    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    part1 = (char*)malloc(M * sizeof(char));
    part2 = (char*)malloc(M * sizeof(char));
    combined = (char*)malloc((M * 2) * sizeof(char));

    MPI_Scatter(str1, M, MPI_CHAR, part1, M, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Scatter(str2, M, MPI_CHAR, part2, M, MPI_CHAR, 0, MPI_COMM_WORLD);
    alternateChars(part1, part2, combined, M);
    MPI_Gather(combined, M * 2, MPI_CHAR, answer, M * 2, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        answer[M * size * 2] = '\0';
        printf("\nAlternated Result: %s\n", answer);

        free(str1);
        free(str2);
        free(answer);
    }

    free(part1);
    free(part2);
    free(combined);

    MPI_Finalize();
    return 0;
}