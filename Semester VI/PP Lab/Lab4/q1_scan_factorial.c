#include <mpi.h>
#include <stdio.h>

int fact(int n) {
    if (n <= 1) return 1;
    else return n * fact(n-1);
}

void handleMPIError(int code) {
    if (code != MPI_SUCCESS) {
        char errstr[MPI_MAX_ERROR_STRING];
        int len, class;

        MPI_Error_class(code, &class);
        // MPI_Error_string(code, errstr, &len);
        // fprintf(stdout, "%d\t%s\n", code, errstr);
        // fflush(stdout);
        MPI_Error_string(class, errstr, &len);
        fprintf(stdout, "%d\t%s\n", class, errstr);
        fflush(stdout);

        MPI_Abort(MPI_COMM_WORLD, code);
    }
}

int main(int argc, char *argv[])
{
    int rank, size, err;
    int f, sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    err = MPI_Comm_size(MPI_COMM_WORLD, &size);
    handleMPIError(err);
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    handleMPIError(err);

    f = fact(rank+1);
    err = MPI_Scan(&f, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    handleMPIError(err);

    if (rank == size-1) {
        fprintf(stdout, "[%d] Sum of all factorials: %d\n", rank, sum);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
