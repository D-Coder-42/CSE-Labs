#include <cuda.h>
#include <stdio.h>

__global__ void multMat_el(int *a, int *b, int *res, int M, int N, int Q) {
    int row = blockIdx.x*blockDim.x + threadIdx.x;
    int col = blockIdx.y*blockDim.y + threadIdx.y;

    if (row >= M || col >= Q) return;

    res[row*Q + col] = 0;
    for (int k = 0; k < N; k++)
        res[row*Q + col] += a[row*N + k] * b[k*Q + col];
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
    
    dim3 block(16,16);
    dim3 grid(ceil(q/(float)block.x),ceil(m/(float)block.y));

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    multMat_el<<<grid,block>>>(d_matA, d_matB, d_res, m, n, q);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaMemcpy(res, d_res, sizeRes, cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < q; j++)
            printf("%d ", res[i*q + j]);
        printf("\n");
    }

    printf("Kernel execution time: %f ms\n", milliseconds);

    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_res);
    free(matA);
    free(matB);
    free(res);
    return 0;
}
