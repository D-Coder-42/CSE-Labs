#include <cuda.h>
#include <stdio.h>
#include <string.h>

#define MAXLEN 32

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void binComplement(char *x, char *y, int *n, int maxlen) {
    int idx = getGlobalIdx();
    if (idx >= *n) return;

    char *in = x + idx*maxlen;
    char *out = y + idx*maxlen;

    for (int j = 0; j < maxlen; j++) {
        char c = in[j];
        if (c == '\0') {
            out[j] = '\0';
            break;
        }
        out[j] = (c == '0'? '1' : '0');
    }
}

int main(void) {
    char (*x)[MAXLEN], (*y)[MAXLEN];
    char *d_x, *d_y;
    int n, *d_n;

    printf("Enter size of vector:\t");
    scanf("%d", &n);
    int size = n * MAXLEN;

    x = (char (*)[MAXLEN]) malloc(size);
    y = (char (*)[MAXLEN]) malloc(size);
    cudaMalloc((void**)&d_x, size);
    cudaMalloc((void**)&d_y, size);
    cudaMalloc((void**)&d_n, sizeof(int));

    printf("Enter vector:\t\t");
    for (int i = 0; i < n; i++)
        scanf("%s", x[i]);

    cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    binComplement<<<ceil(n/(float)MAXLEN),MAXLEN>>>(d_x, d_y, d_n, MAXLEN);
    cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost);
    printf("Complement vector:\t");
    for (int i = 0; i < n; i++)
        printf("%s ", y[i]);
    printf("\n");

    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_n);
    free(x);
    free(y);
    return 0;
}