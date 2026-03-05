#include <cuda.h>
#include <stdio.h>

__global__ void multMat_el(int *a, int *b, int *res, int widthA, int heightA) {
    int j = threadIdx.x;
    int rows = heightA;
    int cols = blockDim.x;

    if (j >= cols) return;

    for (int i = 0; i < rows; i++) {
        res[i*cols + j] = 0;
        for (int k = 0; k < widthA; k++)
            res[i*cols + j] += a[i*widthA + k] * b[k*cols + j];
    }
}

int main(void) {
    int *matA, *matB, *res, m, n, p, q;
    int *d_matA, *d_matB, *d_res;

    printf("Enter number of rows for Mat A: "); scanf("%d", &m);
    printf("Enter number of cols for Mat A: "); scanf("%d", &n);
    printf("Enter number of rows for Mat B: "); scanf("%d", &p);
    printf("Enter number of cols for Mat B: "); scanf("%d", &q);
    
    if (n != p) {
        perror("Matrix dimensional mismatch error");
        exit(EXIT_FAILURE);
    }

    int sizeA = (m*n) * sizeof(int);
    int sizeB = (p*q) * sizeof(int);
    int sizeRes = (m*q) * sizeof(int);

    matA = (int*) malloc(sizeA);
    matB = (int*) malloc(sizeB);
    res = (int*) malloc(sizeRes);
    cudaMalloc((void**)&d_matA, sizeA);
    cudaMalloc((void**)&d_matB, sizeB);
    cudaMalloc((void**)&d_res, sizeRes);
    
    printf("Enter input matrix A:\n");
    for (int i = 0; i < m*n; i++)
        scanf("%d", &matA[i]);
    printf("Enter input matrix B:\n");
    for (int i = 0; i < p*q; i++)
        scanf("%d", &matB[i]);

    cudaMemcpy(d_matA, matA, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, sizeB, cudaMemcpyHostToDevice);
    multMat_el<<<1,q>>>(d_matA, d_matB, d_res, n, m);
    cudaMemcpy(res, d_res, sizeRes, cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < q; j++)
            printf("%d ", res[i*q + j]);
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