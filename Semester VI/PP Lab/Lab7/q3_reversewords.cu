#include <cuda.h>
#include <stdio.h>

#define N 1024

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void reverseWords(char* str, char* res, int len) {
    int i = getGlobalIdx();
    if (i >= len) return;
    if (!(i == 0 || str[i-1] == ' ')) return;

    int wlen = 0;
    while (str[i + wlen] != ' ' && str[i + wlen] != '\0') wlen++;

    for (int j = 0; j < wlen; j++) {
        res[i + j] = str[i + (wlen - j - 1)];
    }
    res[i + wlen] = ' ';
}

int main() {
    char buf[N], *str, *res;
    char *d_str, *d_res;

    printf("Enter string: ");
    fgets(buf, N, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    int strsize = (strlen(buf)+1) * sizeof(char);
    str = (char*) malloc(strsize);
    strcpy(str, buf);
    res = (char*) malloc(strsize);

    cudaMalloc((void**)&d_str, strsize);
    cudaMalloc((void**)&d_res, strsize);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float elapsedTime;

    cudaMemcpy(d_str, str, strsize, cudaMemcpyHostToDevice);
    
    cudaEventRecord(start);
    reverseWords<<<ceil(strlen(str)/256.0),256>>>(d_str, d_res, strlen(str));
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);

    cudaMemcpy(res, d_res, strsize, cudaMemcpyDeviceToHost);
    res[strlen(str)] = '\0';
    printf("Resultant string: %s\n", res);
    printf("Time taken: %f\n", elapsedTime);

    cudaFree(d_str);
    cudaFree(d_res);
    free(str);
    free(res);
    return 0;
}
