import numpy as np 
import time

n = 1024

#create random matrix with values between 0 and 1

matrixA = np.random.rand(n,n)
matrixB = np.random.rand(n,n)

row = 0

#loop 5 times for benchmark
for i in range (5):
    start = time.time()
    
    #multiply matrix
    
    result = np.dot(matrixA,matrixB)
    end = time.time()

    #get the time it took
    inter = end - start

    print('tempo matriz',n,":",inter)