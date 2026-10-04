// Input: n V, then n coin values.  Time O(V*n), Space O(V)
#include <stdio.h>
#define INF 1000000000
int c[100], dp[100001], last[100001];
int main() {
    int n, V;
    scanf("%d %d", &n, &V);
    for (int i = 0; i < n; i++) scanf("%d", &c[i]);
    for (int v = 1; v <= V; v++) {
        dp[v] = INF;                                   // dp[0] = 0 by default
        for (int j = 0; j < n; j++)
            if (c[j] <= v && dp[v - c[j]] + 1 < dp[v])
                dp[v] = dp[v - c[j]] + 1, last[v] = c[j];
    }
    if (dp[V] >= INF) return puts("-1"), 0;
    printf("Min coins = %d, used:", dp[V]);
    for (int v = V; v > 0; v -= last[v]) printf(" %d", last[v]);
    puts("");
}
