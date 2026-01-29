#include <mpi.h>
#include <stdio.h>

double y(double x) {
    return 4.0/(1 + x*x);
}

int main(int argc, char *argv[])
{
    int rank, size;
    double div, area, pi;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    div = 1.0/size;
    area = div * y(((rank+0.5)*div));
    
    MPI_Reduce(&area, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fprintf(stdout, "[%d] Approximation of PI: %.15lf\n", rank, pi);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
