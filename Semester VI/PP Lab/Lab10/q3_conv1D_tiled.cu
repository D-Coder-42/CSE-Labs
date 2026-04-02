#include <cuda.h>
#include <stdio.h>

#define TILE_SIZE 4
#define MAX_MASK_WIDTH 31

__constant__ double d_M[MAX_MASK_WIDTH];

__global__ void convolution1D_tiled(double *N, double *P, int maskWidth, int width) {
    __shared__ double tile[TILE_SIZE + MAX_MASK_WIDTH];
    int tx = threadIdx.x;
    int globalStart = blockIdx.x * TILE_SIZE;
    int globalIdx = globalStart + tx;
    int halo = maskWidth / 2;

    if (globalIdx < width)
        tile[tx + halo] = N[globalIdx];
    else
        tile[tx + halo] = 0;

    if (tx < halo) {
        int rightHaloIdx = globalIdx + TILE_SIZE;
        tile[halo + tx + TILE_SIZE] = (rightHaloIdx < width) ? N[rightHaloIdx] : 0;
    }
    else if (tx >= TILE_SIZE - halo) {
        int leftHaloIdx = globalIdx - TILE_SIZE;
        tile[halo + tx - TILE_SIZE] = (leftHaloIdx >= 0) ? N[leftHaloIdx] : 0;
    }

    __syncthreads();

    if (globalIdx < width) {
        double Pval = 0;
        for (int j = 0; j < maskWidth; j++) {
            Pval += tile[tx + j] * d_M[j];
        }
        P[globalIdx] = Pval;
    }
}

int main(void) {
    double *N, *P, *M;
    double *d_N, *d_P;
    int w, mw;

    printf("Enter size of vector: ");
    scanf("%d", &w);
    printf("Enter size of mask: ");
    scanf("%d", &mw);

    int size = w * sizeof(double);
    int masksize = mw * sizeof(double);

    N = (double*) malloc(size);
    P = (double*) malloc(size);
    M = (double*) malloc(masksize);
    cudaMalloc((void**)&d_N, size);
    cudaMalloc((void**)&d_P, size);

    printf("Enter vector N:\t\t");
    for (int i = 0; i < w; i++) 
        scanf("%lf", &N[i]);
    printf("Enter mask M:\t\t");
    for (int i = 0; i < mw; i++) 
        scanf("%lf", &M[i]);

    cudaMemcpy(d_N, N, size, cudaMemcpyHostToDevice);
    cudaMemcpyToSymbol(d_M, M, masksize);

    dim3 block(TILE_SIZE);
    dim3 grid((w + TILE_SIZE - 1) / TILE_SIZE);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    convolution1D_tiled<<<grid, block>>>(d_N, d_P, mw, w);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    cudaDeviceSynchronize();
    cudaMemcpy(P, d_P, size, cudaMemcpyDeviceToHost);

    printf("Convoluted vector:\t");
    for (int i = 0; i < w; i++) 
        printf("%.0lf ", P[i]);
    printf("\n");

    printf("Kernel execution time: %f ms\n", milliseconds);

    cudaFree(d_N);
    cudaFree(d_P);
    free(N);
    free(M);
    free(P);
    return 0;
}
