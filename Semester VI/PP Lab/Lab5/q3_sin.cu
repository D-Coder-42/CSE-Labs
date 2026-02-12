#include <cuda.h>
#include <stdio.h>

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void sine(double *a, double *b, int *n) {
    int idx = getGlobalIdx();
    if (idx < *n)
        b[idx] = sin(a[idx]);
}

int main(void) {
    double *a, *b;
    double *d_a, *d_b;
    int n, *d_n;

    printf("Enter size of vector: ");
    scanf("%d", &n);
    int size = n * sizeof(double);

    a = (double*) malloc(size);
    b = (double*) malloc(size);
    cudaMalloc((void**)&d_a, size);
    cudaMalloc((void**)&d_b, size);
    cudaMalloc((void**)&d_n, sizeof(int));
    
    printf("Enter vector A (angles in radians): ");
    for (int i = 0; i < n; i++)
        scanf("%lf", &a[i]);

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    sine<<<ceil(n/256.0),256>>>(d_a, d_b, d_n);
    cudaMemcpy(b, d_b, size, cudaMemcpyDeviceToHost);
    printf("Sine vector:\t");
    for (int i = 0; i < n; i++)
        printf("%lf ", b[i]);
    printf("\n");

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_n);
    free(a);
    free(b);
    return 0;
}