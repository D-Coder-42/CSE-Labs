#include <cuda.h>
#include <stdio.h>

__device__ int fact(int n) {
    int f = 1;
    for (int i = 2; i <= n; i++)
        f *= i;
    return f;
}
__device__ int digitsum(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

__global__ void editMat_el(int *mat, int *res) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (i >= rows || j >= cols) return;

    int newval = 0;
    if (i < j) 
        newval = fact(mat[i*cols + j]);
    else if (i > j)
        newval = digitsum(mat[i*cols + j]);
    else
        newval = 0;

    res[i*cols + j] = newval;
}

int main(void) {
    int *mat, *res, n;
    int *d_mat, *d_res;

    printf("Enter side of matrix (N): "); scanf("%d", &n);
    int size = (n*n) * sizeof(int);

    mat = (int*) malloc(size);
    res = (int*) malloc(size);
    cudaMalloc((void**)&d_mat, size);
    cudaMalloc((void**)&d_res, size);
    
    printf("Enter input matrix:\n");
    for (int i = 0; i < n*n; i++)
        scanf("%d", &mat[i]);

    cudaMemcpy(d_mat, mat, size, cudaMemcpyHostToDevice);
    editMat_el<<<n,n>>>(d_mat, d_res);
    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
    for (int i = 0; i < n; i++) {
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