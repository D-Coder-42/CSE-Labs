#include <cuda.h>
#include <stdio.h>

#define MAX_MASK_WIDTH 31
__constant__ double d_M[MAX_MASK_WIDTH];

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void convolution1D(double *N, double *P, int maskWidth, int width) {
    int i = getGlobalIdx();
    if (i > width) return;

    double Pval = 0;
    int Nstart = i - (maskWidth)/2;
    for (int j = 0; j < maskWidth; j++) {
        if (Nstart + j >= 0 && Nstart + j < width)
            Pval += N[Nstart + j]*d_M[j];
    }
    P[i] = Pval;
}

int main(void) {
    double *N, *M, *P;
    double *d_N, *d_P;
    int w, mw;

    printf("Enter size of vector: ");
    scanf("%d", &w);
    int size = w * sizeof(double);
    printf("Enter size of mask: ");
    scanf("%d", &mw);
    int masksize = mw * sizeof(double);

    N = (double*) malloc(size);
    M = (double*) malloc(masksize);
    P = (double*) malloc(size);
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

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    convolution1D<<<ceil(w/256.0),256>>>(d_N, d_P, mw, w);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

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