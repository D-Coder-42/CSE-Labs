#include <cuda.h>
#include <stdio.h>

__global__ void multMat_el(int *a, int *b, int *res, int widthA) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (i >= rows || j >= cols) return;

    res[i*cols + j] = 0;
    for (int k = 0; k < widthA; k++)
        res[i*cols + j] += a[i*widthA + k] * b[k*cols + j];
}

int main(void) {
    int *matA, *matB, *res, m, n, p, q = 1;
    int *d_matA, *d_matB, *d_res;

    printf("Enter number of friends: "); scanf("%d", &m);
    printf("Enter number of items: "); scanf("%d", &p);
    n = p; q = 1;

    int sizeA = (m*n) * sizeof(int);
    int sizeB = (p*q) * sizeof(int);
    int sizeRes = (m*q) * sizeof(int);

    matA = (int*) malloc(sizeA);
    matB = (int*) malloc(sizeB);
    res = (int*) malloc(sizeRes);
    cudaMalloc((void**)&d_matA, sizeA);
    cudaMalloc((void**)&d_matB, sizeB);
    cudaMalloc((void**)&d_res, sizeRes);
    
    printf("Enter price of all items:\n");
    for (int i = 0; i < p*q; i++)
        scanf("%d", &matB[i]);
    for (int i = 0; i < m; i++) {
        printf("Enter quantity of items bought by friend %d:\n", i+1);
        for (int j = 0; j < n; j++)
            scanf("%d", &matA[i*n + j]);
    }

    cudaMemcpy(d_matA, matA, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, sizeB, cudaMemcpyHostToDevice);
    multMat_el<<<m,q>>>(d_matA, d_matB, d_res, n);
    cudaMemcpy(res, d_res, sizeRes, cudaMemcpyDeviceToHost);

    int cost = 0;
    for (int i = 0; i < m; i++)
        cost += res[i];
    printf("Total purchase cost: %d\n", cost);

    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_res);
    free(matA);
    free(matB);
    free(res);
    return 0;
}