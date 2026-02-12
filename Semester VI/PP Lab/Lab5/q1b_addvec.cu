#include <cuda.h>
#include <stdio.h>

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void add(int *a, int *b, int *c) {
    int idx = getGlobalIdx();
    c[idx] = a[idx] + b[idx];
}

int main(void) {
    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    int n;

    printf("Enter size of vector: ");
    scanf("%d", &n);
    int size = n * sizeof(int);

    a = (int*) malloc(size);
    b = (int*) malloc(size);
    c = (int*) malloc(size);
    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_c, size);
    
    printf("Enter vector A: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    printf("Enter vector B: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &b[i]);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);
    add<<<1,n>>>(d_a, d_b, d_c);
    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);
    printf("Sum vector:\t");
    for (int i = 0; i < n; i++)
        printf("%d ", c[i]);
    printf("\n");

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    free(a);
    free(b);
    free(c);
    return 0;
}