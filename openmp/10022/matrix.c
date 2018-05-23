#include "matrix.h"


static void tranpose( int N, unsigned long B[][2048]){
    UINT temp;
    for( int i = 0; i < N; i++){
        for( int j = i+1; j < N; j++){
            temp = B[i][j];
            B[i][j] = B[j][i];
            B[j][i] = temp;
        }
    }
}

void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {
    tranpose(N,B);
    #pragma omp parallel for 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            unsigned long sum = 0;    // overflow, let it go.
            UINT *a = &A[i][0], *b = &B[j][0];
            for (int k = 0; k < N; k++){
                sum += (*a) * (*b);
                a++;
                b++;
            }
            C[i][j] = sum;
        }
    }
}