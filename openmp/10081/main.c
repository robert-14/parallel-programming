#include <stdio.h>
#include <omp.h>

//#define DEBUG

#define MAXN 2048
#define MAXM 1024

char cell[2][MAXN][MAXN];
int N,M;

int main(){
    scanf("%d %d",&N,&M);
   
    for( int i = 1; i <= N; i++){
        scanf("%s",&cell[0][i][1]);
        for( int j = 1; j <= N ; j++){
            cell[0][i][j] -= '0';
        }
        cell[0][i][N+1] = 0;
    }  

    int buf = 0;
    int neighbor = 0;
    int chunk;
    if( N/omp_get_max_threads() < 1) chunk = 1;
    else chunk = N/omp_get_max_threads();
    //printf("%d\n",chunk);

    for( int round = 0; round < M; round++){
#pragma omp parallel for schedule(static,chunk) private(neighbor)
        for( int i = 1; i <= N; i++){
            for( int j = 1; j <= N; j++){
                neighbor = cell[buf][i-1][j-1] + cell[buf][i][j-1] + cell[buf][i+1][j-1] + cell[buf][i-1][j] + cell[buf][i+1][j] +cell[buf][i-1][j+1] + cell[buf][i][j+1] + cell[buf][i+1][j+1];
                cell[!buf][i][j] = ( cell[buf][i][j] == 0 && neighbor == 3) || ( cell[buf][i][j] == 1 && (neighbor ==2 || neighbor ==3));
            }
        }
        buf = !buf;
    }  

    for( int i = 1; i <= N; i++){
        for( int j = 1; j <= N ; j++){
            cell[buf][i][j] += '0';
        }
        cell[buf][i][N+1] = '\0';
        printf("%s\n",&cell[buf][i][1] );
    }   


#ifdef DEBUG    
    for( int i = 0; i <= N+1; i++){
        for( int j = 0; j <= N+1; j++){
            if(j == N+1) printf("%d\n",cell[0][i][j]);
            else printf("%d ",cell[buf][i][j]);
        }
    }
#endif
    return 0;

}