#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"
 
#define MAXN 10000005
#define MAX_THREAD 4
//#define Debug
uint32_t prefix_sum[MAXN];

typedef struct{
    int start,end;
    uint32_t* psum;
    uint32_t key;
}argu;

int min( const int x, const int y){
    return x < y? x:y;
}

int max( const int x, const int y){
    return x > y? x:y;
}

void *task1( void *Argu){
    argu data = *((argu *)Argu);
    int start = data.start;
    int end = data.end;
    uint32_t *psum = data.psum;
    uint32_t key = data.key;
    uint32_t sum = 0;
    int real_index = 0; //because psum already + i

#ifdef Debug
    printf("start = %d, end = %d,key = %d,\n",start,end,key );
#endif

    for( int i = start; i <= end; i++){
        sum += encrypt(i,key);
        psum[real_index] = sum;
        real_index++;
    }
    free(Argu);
    pthread_exit(NULL);
}

void *task2(void *Argu){
    argu data = *((argu *)Argu);
    int start = data.start;
    int end = data.end;
    uint32_t *psum = data.psum;
    uint32_t base = data.key;
    int real_index = 0; //because psum already + i
    for( int i = start; i <= end; i++){
        psum[real_index] += base;
        real_index++;
    }
    free(Argu);
    pthread_exit(NULL);
}

int main() {
    int n;
    uint32_t key;
    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        pthread_t *threads;
        threads = (pthread_t *) malloc(4 * sizeof(pthread_t));
        pthread_attr_t attr;
        pthread_attr_init(&attr); 
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
        //stage 1: solve the prefix sum of each block
        int pid = 0;
        int block_size = n/MAX_THREAD + 1;//prevent that block == 0
#ifdef Debug
        printf("before thread:\n");
        printf("block_size = %d\n", block_size);
#endif
        for( int i = 1; i <= n; i += block_size){
            argu *data = (argu *) malloc(sizeof(argu));
            data->start = i;
            data->end = min(n,i+block_size - 1);
            data->psum = prefix_sum + i;
            data->key = key;
            int error = pthread_create(&threads[pid], &attr, task1, data);
            if(error) printf("thread create fail\n");
            pid++;
        }
        for (int i = 0; i < pid; i++)
            pthread_join(threads[i], NULL);

#ifdef Debug
        printf("prefix_sum:\n");
        for( int i = 1; i <= n; i++){
            printf("%d ",prefix_sum[i]);
        }
#endif

        //stage 2: plus the prefix sum of each block
        uint32_t base = 0;
        pid = 0;
        for( int i = 1; i <= n; i += block_size){
            uint32_t temp = base;
            argu *data = (argu *) malloc(sizeof(argu));
            base += prefix_sum[min(n,i+block_size-1)];
            data->start = i;
            data->end = min(n,i+block_size -1);
            data->psum = prefix_sum + i;
            data->key = temp;
            pthread_create(&threads[pid], &attr, task2, data);
            pid++;
        }
        for (int i = 0; i < pid; i++)
            pthread_join(threads[i], NULL);

        //uint32_t sum = 0;
        /*for (int i = 1; i <= n; i++) {
            sum += encrypt(i, key);
            prefix_sum[i] = sum;
        }*/
        output(prefix_sum, n);
    }
    return 0;
}