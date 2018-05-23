#include <stdio.h>
#define MAXN 2048
#define INF (1LL<<60)
int N;
long long dp[MAXN*MAXN], SZ[MAXN+1];
int main() {
    while (scanf("%d", &N) == 1) {
        for (int i = 0; i <= N; i++)
            scanf("%lld", &SZ[i]);
        for (int i=0; i<N; i++)
            dp[i*N + i] = 0;
#pragma omp parallel firstprivate(N)
        for (int i = 1; i < N; i++) {
#pragma omp for
            for (int j = 0; j < N-i; j++) {
                int l = j, r = j+i;
                long long local = INF;
                long long product = SZ[l]*SZ[r+1];
                long long *dp_start = dp + l*N;
                long long *dp_end = dp + r*N;
                for (int k = l; k < r; k++) {
                    long long t = dp_start[k] + dp_end[k+1] + SZ[k+1]*product; //use the downward matrix dp[l,k] = dp[k,l]
                    if (t < local)
                        local = t;
                }
                dp_start[r] = dp_end[l] = local;
            }
        }
        printf("%lld\n", dp[0*N+N-1]);
    }
    return 0;
}