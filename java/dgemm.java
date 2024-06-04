package java;
import java.util.Random;

public class dgemm {

    public static void main(String[] args) {
        int n = 1024; //define size
        double[][] A = generateRandomMatrix(n, n);
        double[][] B = generateRandomMatrix(n, n);
        double[][] C = new double[n][n]; //result matrix

        for(int i = 0 ; i < 5; i++){
        long start_time = System.nanoTime();
        do_dgemm(n, n, n, A, B, C);
        long end_time = System.nanoTime();

        long elasped_time = end_time - start_time; 
        double elasped_time_in_ms = elasped_time/1_000_000.0;

        System.out.printf("Tempo total para dgemm = %.2f ms%n", elasped_time_in_ms);
        }
    }


    public static double[][] generateRandomMatrix(int rows, int cols) {
        Random random = new Random();
        double[][] matrix = new double[rows][cols];
        //create random matrix with values between 0 and 1
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = random.nextDouble();
            }
        }
        return matrix;
    }

    //multiply matrix
    public static void do_dgemm(int M, int N, int K, double[][] A, double[][] B, double[][] C) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                double sum = 0.0;
                for (int k = 0; k < K; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] += sum;
            }
        }
    }
}

