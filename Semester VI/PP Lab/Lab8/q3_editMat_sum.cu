#include <cuda.h>
#include <stdio.h>

__global__ void editMat_sum(int *mat, int *res) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (i >= rows || j >= cols) return;

    int sum = 0;
    if ((i+j)%2 == 0) 
        for (int k = 0; k < rows; k++)
            sum += mat[k*cols + j];
    else
        for (int k = 0; k < cols; k++)
            sum += mat[i*cols + k];

    res[i*cols + j] = sum;
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
    editMat_sum<<<m,n>>>(d_mat, d_res);
    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
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