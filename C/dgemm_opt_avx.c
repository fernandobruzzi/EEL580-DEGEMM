#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <immintrin.h>
#include <stddef.h>

#define ALIGNMENT 64

void dgemm(size_t n, double* A, double* B, double* C) {
    for (size_t i = 0; i < n; i += 4) {
        for (size_t j = 0; j < n; j++) {
            __m256d c0 = _mm256_load_pd(C + i + j * n);
            for (size_t k = 0; k < n; k++) {
                c0 = _mm256_add_pd(c0,
                    _mm256_mul_pd(_mm256_load_pd(A + i + k * n),
                                  _mm256_set1_pd(B[k + j * n])));
            }
            _mm256_store_pd(C + i + j * n, c0);
        }
    }
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

//instruction to compile: 
//gcc -o codigoavx avx.c -mavx512f -O3