package java;
import java.util.Random;

public class dgemm_cb {

    public static void main(String[] args) {
        int n = 1024; //define size
        double[][] A = generateRandomMatrix(n, n);
        double[][] B = generateRandomMatrix(n, n);
        double[][] C = new double[n][n]; //result matrix

        int blockSize = 32;


        for(int i = 0 ; i < 5; i++){
            long start_time = System.nanoTime();
            do_dgemmcb(n, n, n, A, B, C, blockSize);
            long end_time = System.nanoTime();
    
            long elasped_time = end_time - start_time; 
            double elasped_time_in_ms = elasped_time/1_000_000.0;
    
            System.out.printf("Tempo total para dgemm = %.2f ms%n", elasped_time_in_ms);
            
        }
    }

    public static double[][] generateRandomMatrix(int rows, int cols) {
        Random random = new Random();
        double[][] matrix = new double[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = random.nextDouble(); //create random matrix with values between 0 and 1
            }
        }
        return matrix;
    }

    public static void do_dgemmcb(int M, int N, int K, double[][] A, double[][] B, double[][] C, int blockSize) {

        // dgemm w/cache blocking
        for (int ii = 0; ii < M; ii += blockSize) {
            for (int jj = 0; jj < N; jj += blockSize) {
                for (int kk = 0; kk < K; kk += blockSize) {
                    for (int i = ii; i < Math.min(ii + blockSize, M); i++) {
                        for (int j = jj; j < Math.min(jj + blockSize, N); j++) {
                            double sum = 0.0;
                            for (int k = kk; k < Math.min(kk + blockSize, K); k++) {
                                sum += A[i][k] * B[k][j];
                            }
                            C[i][j] += sum;
                        }
                    }
                }
            }
        }
    }
}
