#include <cuda.h>
#include <stdio.h>

__global__ void transpose(int *arr, int *res) {
    int i = threadIdx.x, j = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (i >= cols || j >= rows) return;
    res[i*rows + j] = arr[j*cols + i];
}

int main(void) {
    int *mat, *res, m, n;
    int *d_mat, *d_res;

    printf("Enter number of rows: "); scanf("%d", &m);
    printf("Enter number of cols: "); scanf("%d", &n);
    int size = (m*n) * sizeof(int);

    mat = (int*) malloc(size);
    res = (int*) malloc(size);
    cudaMalloc((void**)&d_mat, size);
    cudaMalloc((void**)&d_res, size);
    
    printf("Enter input matrix:\n");
    for (int i = 0; i < m*n; i++)
        scanf("%d", &mat[i]);

    cudaMemcpy(d_mat, mat, size, cudaMemcpyHostToDevice);
    transpose<<<m,n>>>(d_mat, d_res);
    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Transpose matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", res[i*m + j]);
        printf("\n");
    }

    cudaFree(d_mat);
    cudaFree(d_res);
    free(mat);
    free(res);
    return 0;
}