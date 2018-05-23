#include "VSQRT.h"
#include <x86intrin.h> 
#include <math.h>
 
void sqrt2(float *begin, float *end) {
    *begin = sqrt(*begin);
    printf("%f\n", *begin);
    /*int len1 = end - begin;
    int len2 = len1 - (len1%8);
    //printf("%d\n",len1);
    //printf("%d\n",len2);
    __m256 X,Y;
    for ( int i = 0; i < len2; i += 8)
        // *begin = sqrt(*begin);
        X = _mm256_load_ps(begin);
        Y = _mm256_sqrt_ps(X);
        _mm256_store_ps(begin,Y);
        for( int j = 0; j <  8;j++){
            printf("%f\n",*(begin + j));
        }    
        begin += 8;
    for ( int i = len2; i < len1; i++){
        *begin = sqrt(*begin);
        begin++;
    }*/

}