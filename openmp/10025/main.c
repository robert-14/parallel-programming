#include <stdio.h>
#include <limits.h>

#define MAXN 512
//#define DEBUG

int A[MAXN][MAXN],B[MAXN][MAXN],C[MAXN][MAXN];

int main(){
    int A_w,A_h,B_w,B_h;
    
    while( ~scanf("%d %d %d %d",&A_h,&A_w,&B_h,&B_w) ){
        for(int i = 0; i < A_h; i++){
            for( int j = 0; j < A_w; j++){
                scanf("%d",&A[i][j]);
            }
        }
        for(int i = 0; i < B_h; i++){
            for( int j = 0; j < B_w; j++){
                scanf("%d",&B[i][j]);
            }
        }
        #pragma omp parallel for
        for( int i = 0; i < A_h - B_h + 1; i++){
            for(int j = 0; j < A_w - B_w + 1; j++){
                int diff = 0;
                for( int k = 0; k < B_h; k++){
                    for( int l = 0; l < B_w; l++){
                        diff += (A[i+k][j+l]-B[k][l])*(A[i+k][j+l]-B[k][l]);
                    }
                }
                C[i][j] = diff;
            }
        }
        int MAX = INT_MAX, ans_x,ans_y;
        for( int i = 0; i < A_h - B_h + 1; i++){
            for( int j = 0; j < A_w - B_w + 1; j++){
                if( C[i][j] < MAX) {
                    MAX = C[i][j];
                    ans_x = i + 1;
                    ans_y = j + 1;
                }    
            }
        }
#ifdef DEBUG
        printf("A_h = %d, A_w = %d, B_h = %d, B_w = %d\n",A_h,A_w,B_h,B_w);
        for(int i = 0; i < A_h; i++){
            for( int j = 0; j < A_w; j++){
                if( j == A_w -1) printf("%d\n",A[i][j]);
                else printf("%d ",A[i][j]);
            }
        }
        for(int i = 0; i < B_h; i++){
            for( int j = 0; j < B_w; j++){
                if( j == B_w -1) printf("%d\n",B[i][j]);
                else printf("%d ",B[i][j]);
            }
        }
        for( int i = 0; i < A_h - B_h + 1; i++){
            for( int j = 0; j < A_w - B_w + 1; j++){
                if( j == A_w -B_w) printf("%d\n",C[i][j]);
                else printf("%d ",C[i][j]);
            }
        }
        
        printf("MAX = %d\n",MAX);

#endif
        printf("%d %d\n",ans_x,ans_y);
    }   


    return 0;

}