#include <stdio.h>
#include <cuda.h>

#define BLOCK_WIDTH 2
#define TILE_WIDTH 2
#define WIDTH 4

__global__ void matMult_tiledEl(int *a, int *b, int *c) {
    __shared__ int Mds[TILE_WIDTH][TILE_WIDTH];
    __shared__ int Nds[TILE_WIDTH][TILE_WIDTH];

    int bx = blockIdx.x, by = blockIdx.y;
    int tx = threadIdx.x, ty = threadIdx.y;
    int row = by*TILE_WIDTH + ty;
    int col = bx*TILE_WIDTH + tx;
    int val = 0;

    for (int m = 0; m < WIDTH/TILE_WIDTH; m++) {
        Mds[ty][tx] = a[row*WIDTH + m*TILE_WIDTH + tx];
        Nds[ty][tx] = b[(m*TILE_WIDTH + ty)*WIDTH + col];

        __syncthreads();
        for (int k = 0; k < TILE_WIDTH; k++) 
            val += Mds[ty][k]*Nds[k][tx];
        __syncthreads(); 
    }

    c[row*WIDTH + col] = val;
}

int main(void) {
    int *matA, *matB, *res;
    int *d_matA, *d_matB, *d_res;

    int size = (WIDTH*WIDTH) * sizeof(int);
    
    matA = (int*) malloc(size);
    matB = (int*) malloc(size);
    res = (int*) malloc(size);
    cudaMalloc((void**)&d_matA, size);
    cudaMalloc((void**)&d_matB, size);
    cudaMalloc((void**)&d_res, size);
    
    printf("Enter input matrix A:\n");
    for (int i = 0; i < WIDTH*WIDTH; i++)
        scanf("%d", &matA[i]);
    printf("Enter input matrix B:\n");
    for (int i = 0; i < WIDTH*WIDTH; i++)
        scanf("%d", &matB[i]);

    cudaMemcpy(d_matA, matA, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_matB, matB, size, cudaMemcpyHostToDevice);
    
    dim3 grid(WIDTH/BLOCK_WIDTH, WIDTH/BLOCK_WIDTH);
    dim3 block(BLOCK_WIDTH, BLOCK_WIDTH);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    matMult_tiledEl<<<grid,block>>>(d_matA, d_matB, d_res);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaMemcpy(res, d_res, size, cudaMemcpyDeviceToHost);
    printf("Resultant matrix:\n");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < WIDTH; j++)
            printf("%d ", res[i*WIDTH + j]);
        printf("\n");
    }

    printf("Kernel execution time: %f ms\n", milliseconds);

    cudaFree(d_matA);
    cudaFree(d_matB);
    cudaFree(d_res);
    free(matA);
    free(matB);
    free(res);
    return 0;
}
