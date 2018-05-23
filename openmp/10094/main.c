#include <stdio.h>
#include <string.h>
#include "omp.h"

int N,M;
int w[10002],v[10002]; 
int c[2][1000002];

int max( const int a, const int b ){
    if( a < b) return b;
    else return a;
}

int main(){
    scanf("%d %d", &N,&M);
    for( int i = 0; i < N; i++){
        scanf("%d %d",&w[i],&v[i]);
    } 
    int xx,yy = 0;
    memset( c, 0, sizeof(c));
    for( int i = 0; i < N ; i++){
        xx = 1 - yy;
#pragma omp parallel for        
        for( int j = 0; j <= M; j++){
            if( j - w[i] < 0) c[xx][j] = c[yy][j];
            else c[xx][j] = max(c[yy][j],c[yy][j-w[i]] + v[i] );      
        }
        yy = 1- yy;
    }
    if( !xx) printf("%d\n",c[0][M]);
    else printf("%d\n",c[1][M]);

    return 0;

}    