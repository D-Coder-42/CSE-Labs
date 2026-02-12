#include <cuda.h>
#include <stdio.h>

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void linalg(int *x, int *y, int *a, int *n) {
    int idx = getGlobalIdx();
    if (idx < *n)
        y[idx] = *a * x[idx] + y[idx];
}

int main(void) {
    int *x, *y, a, n;
    int *d_x, *d_y, *d_a, *d_n;

    printf("Enter size of vector: ");
    scanf("%d", &n);
    int size = n * sizeof(int);

    x = (int*) malloc(size);
    y = (int*) malloc(size);
    cudaMalloc((void**)&d_x, size);
    cudaMalloc((void**)&d_y, size);
    cudaMalloc((void**)&d_a, sizeof(int));
    cudaMalloc((void**)&d_n, sizeof(int));
    
    printf("Enter vector X:\t\t");
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);
    printf("Enter vector Y:\t\t");
    for (int i = 0; i < n; i++)
        scanf("%d", &y[i]);
    printf("Enter scalar multiplier: ");
    scanf("%d", &a);

    cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_a, &a, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    linalg<<<ceil(n/256.0),256>>>(d_x, d_y, d_a, d_n);
    cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost);
    printf("Resultant vector:\t");
    for (int i = 0; i < n; i++)
        printf("%d ", y[i]);
    printf("\n");

    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_a);
    cudaFree(d_n);
    free(x);
    free(y);
    return 0;
}