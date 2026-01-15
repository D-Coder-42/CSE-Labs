#include <mpi.h>
#include <stdio.h>
#include <string.h>

void toggle(char* s, int n) {
    for (int i = 0; i < n; i++) {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
        else if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] += 32;
    }
}

int main (int argc, char *argv[]) {
	int rank, size;
	char string[256];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
    if (rank == 0) {
        fprintf(stdout, "Enter string to send: ");
        fflush(stdout);
        fgets(string, 256, stdin);
        string[strlen(string) - 1] = '\0';
        MPI_Ssend((void *)string, strlen(string)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv((void *)string, 256, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %s (toggled)\n", rank, string);
        fflush(stdout);

    } else {
        MPI_Recv((void *)string, 256, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        fprintf(stdout, "[%d] Received: %s\n", rank, string);
        fflush(stdout);
        toggle(string, strlen(string));
        MPI_Ssend((void *)string, strlen(string)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
    }

	MPI_Finalize();
	return 0;
}
