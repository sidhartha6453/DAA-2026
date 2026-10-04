// Input: n, then n positive integers.  Time O(n^2), Space O(n)
#include <stdio.h>
int a[100000], prev[100000];
long long dp[100000];
int main() {
    int n, b = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    for (int i = 0; i < n; i++) {
        dp[i] = a[i], prev[i] = -1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && dp[j] + a[i] > dp[i]) dp[i] = dp[j] + a[i], prev[i] = j;
        if (dp[i] > dp[b]) b = i;
    }
    printf("Max sum = %lld, subsequence (reversed):", dp[b]);
    for (int i = b; i >= 0; i = prev[i]) printf(" %d", a[i]);
    puts("");
}
