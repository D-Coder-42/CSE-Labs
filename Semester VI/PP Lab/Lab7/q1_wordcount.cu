#include <cuda.h>
#include <stdio.h>

#define N 1024

__device__ int getGlobalIdx() {
    int blockID = blockIdx.x + gridDim.x*blockIdx.y + gridDim.x*gridDim.y*blockIdx.z;
    int threadID = blockID*(blockDim.x*blockDim.y*blockDim.z) + threadIdx.z*(blockDim.x*blockDim.y) + threadIdx.y*blockDim.x + threadIdx.x;
    return threadID;
}

__global__ void wordCount(char* str, char* word, int* d_count, int len, int wlen) {
    int i = getGlobalIdx();
    if (i > (len - wlen)) return;

    if ((i == 0 || str[i-1] == ' ') && (str[i+wlen] == ' ' || str[i+wlen] == '\0')) {
        for (int j = 0; j < wlen; j++)
            if (str[i+j] != word[j]) return;
        atomicAdd(d_count, 1);
    }
}

__host__ void handleCudaError() {
    cudaError_t error = cudaGetLastError();
    if (error != cudaSuccess)
        printf("CUDA Error: %s\n", cudaGetErrorString(error));
}

int main() {
    char buf[N], *str, *word;
    char *d_str, *d_word;
    int *count, *d_count;

    printf("Enter string: ");
    fgets(buf, N, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    int strsize = strlen(buf) * sizeof(char);
    str = (char*) malloc(strsize);
    strcpy(str, buf);
    printf("Enter word to find: ");
    fgets(buf, N, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    int wordsize = strlen(buf) * sizeof(char);
    word = (char*) malloc(wordsize);
    strcpy(word, buf);

    count = (int*) malloc(sizeof(int));
    cudaMalloc((void**)&d_str, strsize);
    cudaMalloc((void**)&d_word, wordsize);
    cudaMalloc((void**)&d_count, sizeof(int));
    *count = 0;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    float elapsedTime;

    cudaMemcpy(d_str, str, strsize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_word, word, wordsize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_count, count, sizeof(int), cudaMemcpyHostToDevice);
    handleCudaError();
    
    cudaEventRecord(start);
    wordCount<<<ceil(strlen(str)/256.0),256>>>(d_str, d_word, d_count, strlen(str), strlen(word));
    handleCudaError();
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);

    cudaMemcpy(count, d_count, sizeof(int), cudaMemcpyDeviceToHost);
    printf("Total occurences of '%s' = %d\n", word, *count);
    printf("Time taken: %f\n", elapsedTime);

    cudaFree(d_str);
    cudaFree(d_count);
    free(str);
    free(count);
    return 0;
}