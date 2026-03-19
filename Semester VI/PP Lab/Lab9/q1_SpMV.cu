#include <cuda.h>
#include <stdio.h>

__host__ void matToCSR(int *mat, int m, int n, int *data, int *col_idx, int *row_ptr) {
    int ct = 0;
    for (int i = 0; i < m; i++) row_ptr[i] = -1;
    for (int i = 0;  i < m*n; i++) {
        if (mat[i] != 0) {
            data[ct] = mat[i];
            col_idx[ct] = i%n;
            if (row_ptr[i/n] == -1) row_ptr[i/n] = ct;
            ct++;
        }
    }
}

__global__ void mult_SpMV(int *data, int *col_idx, int *row_ptr, int *vec, int *res, int ct, int m) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < m) {
        int row_start = row_ptr[row];
        int row_end = (row == m-1) ? ct : row_ptr[row+1];
        int sum = 0;
        if (row_start != -1)
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
    int vecSize = m * sizeof(int);

    mat = (int*) malloc(matSize);
    vec = (int*) malloc(vecSize);
    res = (int*) malloc(vecSize);

    printf("Enter input matrix:\n");
    for (int i = 0; i < m*n; i++) {
        scanf("%d", &mat[i]);
        if (mat[i] != 0) ct++;
    }
    printf("Enter input vector:\n");
    for (int i = 0; i < m; i++)
        scanf("%d", &vec[i]);

    int csrSize = ct * sizeof(int);
    data = (int*) malloc(csrSize);
    col_idx = (int*) malloc(csrSize);
    row_ptr = (int*) malloc(vecSize);

    matToCSR(mat, m, n, data, col_idx, row_ptr);

    cudaMalloc((void**)&d_data, csrSize);
    cudaMalloc((void**)&d_colidx, csrSize);
    cudaMalloc((void**)&d_rowptr, vecSize);
    cudaMalloc((void**)&d_vec, vecSize);
    cudaMalloc((void**)&d_res, vecSize);

    cudaMemcpy(d_data, data, csrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_colidx, col_idx, csrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_rowptr, row_ptr, vecSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_vec, vec, vecSize, cudaMemcpyHostToDevice);
    mult_SpMV<<<1,m>>>(d_data, d_colidx, d_rowptr, d_vec, d_res, ct, m);
    cudaMemcpy(res, d_res, vecSize, cudaMemcpyDeviceToHost);
    
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