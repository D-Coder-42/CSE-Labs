#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void duplicate(char c, int n, char* ret) {
    for (int i = 0; i < n; i++) ret[i] = c;
}

int getIndex(int n) {
    return (n*(n+1))/2;
}

int main(int argc, char *argv[])
{
    int rank, size;
    char c, *str, *ret, *ans;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        str = (char*)malloc(size * sizeof(char));
        fprintf(stdout, "Enter string of length %d: ", size);
        fflush(stdout);
        fgets(str,sizeof(str),stdin);
        ans = (char*)malloc((getIndex(size)+1) * sizeof(char));
    }

    ret = (char*)malloc((rank+1) * sizeof(char));
    MPI_Scatter(str, 1, MPI_CHAR, &c, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    duplicate(c, rank+1, ret);

    if (rank != 0) {
        MPI_Send(ret, rank+1, MPI_CHAR, 0, rank, MPI_COMM_WORLD);
    } else {
        strcpy(ans, ret);
        for (int i = 1; i < size; i++) {
            MPI_Recv(ans+getIndex(i), i+1, MPI_CHAR, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        ans[getIndex(size)] = '\0';
        fprintf(stdout, "[%d] Output string: %s\n", rank, ans);
        fflush(stdout);
        
        free(str);
        free(ans);
    }

    free(ret);
    MPI_Finalize();
    return 0;
}
