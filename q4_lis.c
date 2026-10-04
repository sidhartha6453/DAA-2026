// Input: n, then n integers.  DP: O(n^2).  Tails+binary search: O(n log n)
#include <stdio.h>
int a[100000], dp[100000], t[100000];
int main() {
    int n, best = 0, len = 0;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    for (int i = 0; i < n; i++) {
        dp[i] = 1;
        for (int j = 0; j < i; j++)
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) dp[i] = dp[j] + 1;
        if (dp[i] > best) best = dp[i];
        int lo = 0, hi = len;                          // O(n log n) version
        while (lo < hi) { int m = (lo + hi) / 2; if (t[m] < a[i]) lo = m + 1; else hi = m; }
        t[lo] = a[i];
        if (lo == len) len++;
    }
    printf("LIS = %d (O(n^2)), %d (O(n log n))\n", best, len);
}
