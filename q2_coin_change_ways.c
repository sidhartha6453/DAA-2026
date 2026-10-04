// Input: n V, then n coin values.  Time O(V*n), Space O(V)
#include <stdio.h>
unsigned long long dp[100001] = {1};                   // dp[0] = 1
int main() {
    int n, V, c;
    scanf("%d %d", &n, &V);
    while (n--) {                                      // coin loop OUTSIDE => order ignored
        scanf("%d", &c);
        for (int v = c; v <= V; v++) dp[v] += dp[v - c];
    }
    printf("Ways = %llu\n", dp[V]);
}
