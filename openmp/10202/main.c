#include <stdio.h>
#include <string.h>
#include "omp.h"
#include <stdlib.h>

//#define DEBUG

char obj[17][17];
int N;


int ok( int ans[], int col, int test){
    if( obj[test][col] == '*') return 0;
    for( int i = 0; i < col; i++){
        if( (ans[i] == test) ||  (abs(test - ans[i]) == col - i) ) return 0;
    }
    return 1;
}



int goqueen( int ans[], int next){

    if( next >= N) {
        return 1;    
    }
    int cur_sum = 0;
    for( int i = 0; i < N; i++){
        if( ok(ans,next,i)){
            ans[next] = i;
            cur_sum += goqueen(ans,next+1);
        }
    }
    return cur_sum;
    
}


int main(){
    int case1 = 1;
    int ans[16];
    while ( ~scanf("%d",&N)) {
        memset(ans,0,sizeof(ans));
        int num = 0;
        for ( int i = 0; i < N; i++){
                scanf("%s",obj[i]);
        }

#pragma omp parallel for collapse(3) private(ans) schedule(dynamic,1) reduction(+:num)
        for( int i = 0; i < N; i++){
            for( int j = 0; j < N; j++){
                for( int k = 0; k < N; k++){
                    if( (ok(ans,0,i) )){
                        ans[0] = i;
                        if(ok(ans,1,j)){
                            ans[1] = j;
                            if(ok(ans,2,k)) {
                                ans[2] = k;
                                num += goqueen(ans,3);
                            }
                        }
                    }

                }
            }
        }


#ifdef DEBUG
        for ( int i = 0; i < N; i++){
            
                //printf("obj[%d][%d] = %c \n ",i,j,obj[i][j]);

                printf("%s ",obj[i]);
        }
#endif
        printf("Case %d: %d\n", case1, num);
        case1++;
    }
    return 0;
}    