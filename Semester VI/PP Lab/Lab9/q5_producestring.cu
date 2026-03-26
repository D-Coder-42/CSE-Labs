#include <cuda.h>
#include <stdio.h>

__global__ void produceString(char *str, int *count, int *start, char *res) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (i >= rows || j >= cols) return;

    for (int k = 0; k < count[i*cols + j]; k++)
        res[start[i*cols + j] + k] = str[i*cols + j];
}

int main(void) {
    int *matB, *count, *d_matB, *d_count, m, n;
    char *matA, *res, *d_matA, *d_res;

    printf("Enter number of rows: "); scanf("%d", &m);
    printf("Enter number of cols: "); scanf("%d", &n);
    int size = m*n, tot = 0;

    matA = (char*) malloc(size * sizeof(char));
    matB = (int*) malloc(size * sizeof(int));
    count = (int*) malloc(size * sizeof(int));
    cudaMalloc((void**)&d_matA, size * sizeof(char));
    cudaMalloc((void**)&d_matB, size * sizeof(int));
    cudaMalloc((void**)&d_count, size * sizeof(int));
    
    printf("Enter input string matrix:\n");
    for (int i = 0; i < m*n; i++)
        scanf(" %c", &matA[i]);
    // getchar();
    count[0] = 0;
    printf("Enter input count matrix:\n");
    for (int i = 0; i < m*n; i++) {
        scanf("%d", &matB[i]);
        if (i > 0) count[i] = count[i-1] + matB[i-1];
        tot += matB[i];
    }

    res = (char*) malloc(tot * sizeof(char));
    cudaMalloc((void**)&d_res, tot * sizeof(char));

    cudaMemcpy(d_matA, matA, size * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_count, count, size * sizeof(int), cudaMemcpyHostToDevice);
    produceString<<<m,n>>>(d_matA, d_matB, d_count, d_res);
    cudaMemcpy(res, d_res, tot * sizeof(char), cudaMemcpyDeviceToHost);
    printf("Resultant string:\n");
    for (int i = 0; i < tot; i++)
        printf("%c", res[i]);
    printf("\n");

    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_count);
    cudaFree(d_res);
    free(matA);
    free(matB);
    free(count);
    free(res);
    return 0;
}