#include <cuda.h>
#include <stdio.h>

__global__ void rowPower(int *arr) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (j >= cols || i >= rows) return;
    arr[i*cols + j] = powf(arr[i*cols + j], i+1);
}

int main(void) {
    int *mat, m, n;
    int *d_mat;

    printf("Enter number of rows: "); scanf("%d", &m);
    printf("Enter number of cols: "); scanf("%d", &n);
    int size = (m*n) * sizeof(int);

    mat = (int*) malloc(size);
    cudaMalloc((void**)&d_mat, size);
    
    printf("Enter input matrix:\n");
    for (int i = 0; i < m*n; i++)
        scanf("%d", &mat[i]);

    cudaMemcpy(d_mat, mat, size, cudaMemcpyHostToDevice);
    rowPower<<<m,n>>>(d_mat);
    cudaMemcpy(mat, d_mat, size, cudaMemcpyDeviceToHost);
    printf("Updated matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", mat[i*n + j]);
        printf("\n");
    }

    cudaFree(d_mat);
    free(mat);
    return 0;
}