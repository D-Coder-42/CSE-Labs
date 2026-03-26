#include <cuda.h>
#include <stdio.h>

__device__ int pow(int x, int y) {
    int res = 1;
    for (int i = 0; i < y; i++)
        res *= x;
    return res;
}

__device__ int decToBin(int num) {
    int n = num, pos = 0, res = 0;
    while (n > 0) {
        res += (n%2) * pow(10,pos);
        n /= 2;
        pos++;
    }
    return res;
}

__global__ void onescomp(int *arr, int *res) {
    int j = threadIdx.x, i = blockIdx.x;
    int rows = gridDim.x, cols = blockDim.x;

    if (j >= cols || i >= rows) return;

    if ((j == 0 || j == cols-1) || (i == 0 || i == rows-1))
        res[i*cols + j] = arr[i*cols + j];
    else {
        int val = arr[i*cols + j], ct = 0;
        while (val > 0) {
            ct++;
            val >>= 1;
        }
        res[i*cols + j] = decToBin((~arr[i*cols + j]) & (pow(2,ct) - 1));
    }
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
    onescomp<<<m,n>>>(d_mat, d_res);
    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Updated matrix:\n");
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            printf("%d\t", res[i*n + j]);
        printf("\n");
    }

    cudaFree(d_mat);
    cudaFree(d_res);
    free(mat);
    free(res);
    return 0;
}