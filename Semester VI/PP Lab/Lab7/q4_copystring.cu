#include <cuda.h>
#include <stdio.h>

#define N 1024

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void manipulateString(char* str, char* res, int len, int n) {
    int i = getGlobalIdx();
    if (i > n) return;

    for (int j = 0; j < len; j++)
        res[i*len + j] = str[j];
}

int main() {
    char buf[N], *str, *res;
    char *d_str, *d_res;
    int n;

    printf("Enter string: ");
    fgets(buf, N, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    int strsize = strlen(buf) * sizeof(char);
    str = (char*) malloc(strsize);
    strcpy(str, buf);
    printf("Enter N: ");
    scanf("%d", &n);
    int ressize = strlen(str)*n * sizeof(char);
    res = (char*) malloc(ressize);

    cudaMalloc((void**)&d_str, strsize);
    cudaMalloc((void**)&d_res, ressize);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float elapsedTime;

    cudaMemcpy(d_str, str, strsize, cudaMemcpyHostToDevice);
    
    cudaEventRecord(start);
    manipulateString<<<ceil(strlen(str)/256.0),256>>>(d_str, d_res, strlen(str), n);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);

    cudaMemcpy(res, d_res, ressize, cudaMemcpyDeviceToHost);
    printf("Resultant string: %s\n", res);
    printf("Time taken: %f\n", elapsedTime);

    cudaFree(d_str);
    cudaFree(d_res);
    free(str);
    free(res);
    return 0;
}