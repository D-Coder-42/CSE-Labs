#include <cuda.h>
#include <stdio.h>

__global__ void addMat_row(int *a, int *b, int *res, int rows) {
    int j = threadIdx.x;
    int cols = blockDim.x;

    if (j >= cols) return;
    for (int i = 0; i < rows; i++)
        res[i*cols + j] = a[i*cols + j] + b[i*cols + j];
}

int main(void) {
    int *matA, *matB, *res, m, n;
    int *d_matA, *d_matB, *d_res;

    printf("Enter number of rows: "); scanf("%d", &m);
    printf("Enter number of cols: "); scanf("%d", &n);
    int size = (m*n) * sizeof(int);

    matA = (int*) malloc(size);
    matB = (int*) malloc(size);
    res = (int*) malloc(size);
    cudaMalloc((void**)&d_matA, size);
    cudaMalloc((void**)&d_matB, size);
    cudaMalloc((void**)&d_res, size);
    
    printf("Enter input matrix A:\n");
    for (int i = 0; i < m*n; i++)
        scanf("%d", &matA[i]);
    printf("Enter input matrix B:\n");
    for (int i = 0; i < m*n; i++)
        scanf("%d", &matB[i]);

    cudaMemcpy(d_matA, matA, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, size, cudaMemcpyHostToDevice);
    addMat_row<<<1,n>>>(d_matA, d_matB, d_res, m);
    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", res[i*n + j]);
        printf("\n");
    }

    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_res);
    free(matA);
    free(matB);
    free(res);
    return 0;
}