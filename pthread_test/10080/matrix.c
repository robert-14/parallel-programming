#include "matrix.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 2048

typedef struct argu {
    int start, end;
    UINT *A, *B, *C;
    int N;
} argu;

static void change_matrix( int len,unsigned long B[][2048]){   //tranpose B
    UINT temp;
    for( int i = 0; i < len; i++){
        for( int j = i+1; j < len; j++){
            temp = B[i][j];
            B[i][j] = B[j][i];
            B[j][i] = temp;
        }
    }
    
}

void* task(void *arg) {
    argu data = * ((argu *) arg);
    //printf("hi\n");
    int l = data.start, r = data.end;
    int N = data.N;
    UINT *A = data.A, *B = data.B, *C = data.C;
    UINT stkA[MAXN]; //row
    for (int i = l; i <= r; i++) {
        memcpy(stkA, A + (i * MAXN), sizeof(UINT) * N);
        for (int j = 0; j < N; j++) {
            UINT sum = 0;
            UINT *a = stkA;
            UINT *b = B + (j * MAXN); //a => row, b => column
            int k = 0;
            while( k < N){
                sum += (*a) * (*b);
                a++;
                b++;
                k++;
                //printf("hi\n");
            }
            //printf("sum = %lu\n",sum);
            *(C + (i * MAXN + j)) = sum;
        }
    }
    pthread_exit(NULL);
}
void multiply(int N, UINT A[][2048], UINT B[][2048], UINT C[][2048]) {
    
    change_matrix(N, B);
    pthread_t *threads;
    threads = (pthread_t *) malloc(8 * sizeof(pthread_t));

    pthread_attr_t attr;
    pthread_attr_init(&attr); 
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    for (int i = 0; i < 8; i++) {
        argu *data = (argu *) malloc(sizeof(argu));
        data->start = i *  N/8 ; //start posi of this thread
        data->end = (i+1) *  N/8  - 1; //end position
        data->N = N;
        data->A = &A[0][0];
        data->B = &B[0][0];
        data->C = &C[0][0];
        int error = pthread_create(&threads[i], &attr, task, (void *) (data));
        //if(error) {
        //    printf("create fail\n");
        //}
    }
 
    for (int i = 0; i < 8; i++) 
        pthread_join(threads[i], NULL);
    return ;
}