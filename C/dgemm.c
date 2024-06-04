#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//multiply matrix
void dgemm(size_t n, double* A, double* B, double* C) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            for (size_t k = 0; k < n; k++) {
                C[(i * n) + j] += A[(i * n) + k] * B[(k * n) + j];
            }
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
    if (argc < 2) {
        printf("Por favor, forneça o tamanho da matriz como argumento de linha de comando.\n");
        return 1;
    }

    size_t n = (size_t)atoi(argv[1]);
    if (n <= 0) {
        printf("Por favor, forneça um tamanho válido para a matriz.\n");
        return 1;
    }

    double* A = (double*)malloc(n * n * sizeof(double));
    double* B = (double*)malloc(n * n * sizeof(double));
    double* C = (double*)malloc(n * n * sizeof(double));

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
    free(A);
    free(B);
    free(C);

    return 0;
}

//instruction to compile: 
//gcc -o codigoavx dgemm.c -00 
//gcc -o codigoavx dgemm.c -01 
//gcc -o codigoavx dgemm.c -02
//gcc -o codigoavx dgemm.c -03