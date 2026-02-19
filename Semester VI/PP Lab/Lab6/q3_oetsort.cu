#include <cuda.h>
#include <stdio.h>

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__device__ void phase(int *arr, int *n, int mod) {
    int idx = getGlobalIdx();
    int i = 2 * idx + mod;
    if (i + 1 >= *n) return;

    if (arr[i] > arr[i+1]) {
        int temp = arr[i];
        arr[i] = arr[i+1];
        arr[i+1] = temp;
    }
}

__global__ void sort(int *arr, int *n) {
    for (int i = 0; i < *n; i++)
        phase(arr, n, i%2);
}

int main(void) {
    int *x, n;
    int *d_x, *d_n;

    printf("Enter size of vector: ");
    scanf("%d", &n);
    int size = n * sizeof(int);

    x = (int*) malloc(size);
    cudaMalloc((void**)&d_x, size);
    cudaMalloc((void**)&d_n, sizeof(int));
    
    printf("Enter vector:\t");
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);

    cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    sort<<<ceil(n/256.0),256>>>(d_x, d_n);
    cudaMemcpy(x, d_x, size, cudaMemcpyDeviceToHost);
    printf("Sorted vector:\t");
    for (int i = 0; i < n; i++)
        printf("%d ", x[i]);
    printf("\n");

    cudaFree(d_x);
    cudaFree(d_n);
    free(x);
    return 0;
}