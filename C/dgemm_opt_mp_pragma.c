#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <immintrin.h>
#include <stddef.h>

#define ALIGNMENT 64
#define UNROLL (4)
#define BLOCK_SIZE 32

//create block
void dgemm_block(int n, int si, int sj, int sk, double* A, double* B, double* C){
  for (int i = si; i < si + BLOCK_SIZE; i+= UNROLL*4)
    for (int j = sj; j < sj + BLOCK_SIZE; j++) {
      __m256d c[UNROLL];
      for (int r = 0; r < UNROLL; r++)
        c[r] = _mm256_load_pd(C + (j * n) + i + r * 4);

      for (int k = sk; k < sk + BLOCK_SIZE; k++)
      {
        __m256d bb = _mm256_broadcast_sd(B + (j * n) + k);
        for (int r = 0; r < UNROLL; r++)
          c[r] = _mm256_add_pd(c[r], _mm256_mul_pd(_mm256_load_pd(A + (n * k) + i + 4 * r),bb));
      }
      for (int r = 0; r < UNROLL; r++)
        _mm256_store_pd(C + (j * n) + i + r * 4, c[r]);
    }
}

//multiply matrix
void dgemm(int n, double* A, double* B, double* C){
#pragma omp parallel for
  for (int sj = 0; sj < n; sj += BLOCK_SIZE)
    for (int si = 0; si < n; si += BLOCK_SIZE)
      for (int sk = 0; sk < n; sk += BLOCK_SIZE)
        dgemm_block(n, si, sj, sk, A, B, C);
}

//create random matrix with values between 0 and 1
void make_rand_matrix(size_t n, double* A) {
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < n * n; i++) {
        A[i] = (double)rand() / RAND_MAX;
    }
}

//initialize 0 matrix
void initialize_matrix_with_zeros(size_t n, double* C) {
    for (size_t i = 0; i < n * n; i++) {
        C[i] = 0;
    }
}

int main(int argc, char* argv[]) {
    for (int count = 1; count <= 5; count++){
    if (argc < 2) {
        printf("Por favor, forneça o tamanho da matriz como argumento de linha de comando.\n");
        return 1;
    }

    size_t n = (size_t)atoi(argv[1]);
    if (n <= 0 || n % 8 != 0) {
        printf("Por favor, forneça um tamanho válido para a matriz (múltiplo de 8).\n");
        return 1;
    }

    double* A = (double*)_mm_malloc(n * n * sizeof(double), ALIGNMENT);
    double* B = (double*)_mm_malloc(n * n * sizeof(double), ALIGNMENT);
    double* C = (double*)_mm_malloc(n * n * sizeof(double), ALIGNMENT);


    if (!A || !B || !C) {
        printf("Falha ao alocar memória para matrizes.\n");
        return 1;
    }

    //start the random matrices
    make_rand_matrix(n, A);
    make_rand_matrix(n, B);

    //result matrix
    initialize_matrix_with_zeros(n, C);

    //time the matrix multiply
    clock_t start = clock();
    dgemm(n, A, B, C);
    clock_t stop = clock();

    //get the time it took
    double elapsed_time = (double)(stop - start) / CLOCKS_PER_SEC * 1000;
    printf("Tempo total para dgemm = %.2f ms\n", elapsed_time);

    //free allocated memory
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);
    }
    return 0;
}