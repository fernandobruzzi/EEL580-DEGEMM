import random
import time

#define size
n = 8192

#create random matrix with values between 0 and 1
matrix_a = [[random.random() for _ in range(n)] for _ in range(n)]
matrix_b = [[random.random() for _ in range(n)] for _ in range(n)]
matrix_resultados = [[0 for _ in range(n)] for _ in range(n)]

def multiplicacao_matriz(x, y, z):
    s_time, s_time_cpu = time.time(), time.process_time()
    
    #multiply  matrix
    for i in range(n):
        for j in range(n):
            for k in range(n):
                z[i][j] += x[i][k] * y[k][j]

    #get the time it took
    e_time, e_time_cpu = time.time(), time.process_time()
    tempo_total = e_time - s_time
    tempo_cpu = e_time_cpu - s_time_cpu
    return z, tempo_total, tempo_cpu

resultado, tempo_total, tempo_cpu = multiplicacao_matriz(matrix_a, matrix_b, matrix_resultados)

print("\nTempo decorrido na multiplicação de matrizes: {:.2f} segundos".format(tempo_total))
print("\nTempo decorrido (CPU) na multiplicação de matrizes: {:.2f} segundos".format(tempo_cpu))

