#include <stdio.h>
#include <math.h>

int main(){
    int N,a[1000000],b[10];// I use b[10] to store the total prefix sum of each processor 
    scanf("%d",&N);
    for ( int i = 0; i < N; i++){
        scanf("%d",&a[i]);
    }
    int p = 10;

    for( int i = 0; i < p; i++){         //step1 :Each processor computes its prefix sum.   
        for( int j = 0; j < (N/p); j++){
            if( !j) continue;
            a[i*(N/p) + j] += a[i*(N/p) + j - 1];        
        }
    }
   
    for( int i = 0; i < p; i++){
        b[i] = a[(i+1)*(N/p) - 1];
    } 
    for( int i = 0; i < 4; i++){        //Use the previous algorithm to compute the prefix sum of the last elements from all processors.
        for( int j = pow(2,i); j < p; j++){
            for( int k = 0; k < (N/p); k++){
                a[j*(N/p) + k] += b[j - (int)pow(2,i)];        
            }
           
        }
        for( int k = 1; k < p; k++){      //update b after entire round
            b[k] = a[(k+1)*(N/p) - 1];
        }
    }
    
    for ( int i = 0; i < N; i++){
        if( i == N-1){
            printf("%d\n",a[i]);
            break;
        }    
        printf("%d ",a[i]);
    }
    return 0;

}    