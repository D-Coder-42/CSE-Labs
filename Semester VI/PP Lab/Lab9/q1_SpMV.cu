#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>

__host__ void matToCSR(int *mat, int m, int n, int *data, int *col_idx, int *row_ptr) {
    int ct = 0;
    row_ptr[0] = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (mat[i*n+j] != 0) {
                data[ct] = mat[i*n+j];
                col_idx[ct] = j;
                ct++;
            }
        }
        row_ptr[i+1] = ct;
    }
}

__global__ void mult_SpMV(int *data, int *col_idx, int *row_ptr, int *vec, int *res, int m) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < m) {
        int row_start = row_ptr[row];
        int row_end = row_ptr[row+1];
        int sum = 0;
        for (int j = row_start; j < row_end; j++)
            sum += data[j] * vec[col_idx[j]];
        res[row] = sum;
    }
}

int main(void) {
    int *mat, *data, *col_idx, *row_ptr, *vec, *res;
    int *d_data, *d_colidx, *d_rowptr, *d_vec, *d_res;
    int m, n, ct = 0;

    printf("Enter number of rows: "); scanf("%d", &m);
    printf("Enter number of cols: "); scanf("%d", &n);
    
    int matSize = (m*n) * sizeof(int);
    int vecSize = n * sizeof(int);
    int resSize = m * sizeof(int);

    mat = (int*) malloc(matSize);
    vec = (int*) malloc(vecSize);
    res = (int*) malloc(resSize);

    printf("Enter input matrix:\n");
    for (int i = 0; i < m*n; i++) {
        scanf("%d", &mat[i]);
        if (mat[i] != 0) ct++;
    }
    printf("Enter input vector:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &vec[i]);

    int csrSize = ct * sizeof(int);
    int rowPtrSize = (m+1) * sizeof(int);
    
    data = (int*) malloc(csrSize);
    col_idx = (int*) malloc(csrSize);
    row_ptr = (int*) malloc(rowPtrSize);

    matToCSR(mat, m, n, data, col_idx, row_ptr);

    cudaMalloc((void**)&d_data, csrSize);
    cudaMalloc((void**)&d_colidx, csrSize);
    cudaMalloc((void**)&d_rowptr, rowPtrSize);
    cudaMalloc((void**)&d_vec, vecSize);
    cudaMalloc((void**)&d_res, resSize);

    cudaMemcpy(d_data, data, csrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_colidx, col_idx, csrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_rowptr, row_ptr, rowPtrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vec, vec, vecSize, cudaMemcpyHostToDevice);
    
    mult_SpMV<<<1,m>>>(d_data, d_colidx, d_rowptr, d_vec, d_res, m);
    
    cudaMemcpy(res, d_res, resSize, cudaMemcpyDeviceToHost);
    
    printf("Resultant vector:\n");
    for (int i = 0; i < m; i++)
        printf("%d ", res[i]);
    printf("\n");

    cudaFree(d_data);
    cudaFree(d_colidx);
    cudaFree(d_rowptr);
    cudaFree(d_vec);
    cudaFree(d_res);
    free(mat);
    free(data);
    free(col_idx);
    free(row_ptr);
    free(vec);
    free(res);
    return 0;
}
