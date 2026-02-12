#include <cuda.h>
#include <stdio.h>

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void sort(int *x, int *y, int *n) {
    int idx = getGlobalIdx();
    if (idx < *n) {
        int val = x[idx];
        int rank = 0;
       
        for (int j = 0; j < *n; j++) {
            if (x[j] < val || (x[j] == val && j < idx))
                rank++;
        }
        y[rank] = val;
    }
}

int main(void) {
    int *x, *y, n;
    int *d_x, *d_y, *d_n;

    printf("Enter size of vector: ");
    scanf("%d", &n);
    int size = n * sizeof(int);

    x = (int*) malloc(size);
    y = (int*) malloc(size);
    cudaMalloc((void**)&d_x, size);
    cudaMalloc((void**)&d_y, size);
    cudaMalloc((void**)&d_n, sizeof(int));
    
    printf("Enter vector:\t");
    for (int i = 0; i < n; i++)
        scanf("%d", &x[i]);

    cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    sort<<<ceil(n/256.0),256>>>(d_x, d_y, d_n);
    cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost);
    printf("Sorted vector:\t");
    for (int i = 0; i < n; i++)
        printf("%d ", y[i]);
    printf("\n");

    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_n);
    free(x);
    return 0;
}