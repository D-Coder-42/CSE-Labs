#include <cuda.h>
#include <stdio.h>

__global__ void rowPower(int *arr, int *res) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (j >= cols || i >= rows) return;
    int rowsum = 0, colsum = 0;
    for (int k = 0; k < max(rows, cols); k++) {
        if (k < cols) rowsum += arr[i*cols + k];
        if (k < rows) colsum += arr[k*cols + j];
    }
    res[i*cols + j] = rowsum + colsum;
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
    rowPower<<<m,n>>>(d_mat, d_res);
    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Updated matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", res[i*n + j]);
        printf("\n");
    }

    cudaFree(d_mat);
    cudaFree(d_res);
    free(mat);
    free(res);
    return 0;
}