/* Q9: Optimal alphabetic binary tree (Hu-Tucker family; implemented with the equivalent Garsia-Wachs procedure).
   Phase 1: repeatedly merge the first "locally minimal" adjacent pair and slide the new node left.
   Phase 2: read the leaf depths.  Phase 3: rebuild an alphabetic tree/codes with exactly those depths.
   Validation: cost is compared with the classic O(n^3) interval DP for optimal alphabetic trees. */
#include <stdio.h>
#include <stdlib.h>
#define MAXN 200
#define INF (1LL << 60)
long long wt[2 * MAXN]; int L[2 * MAXN], R[2 * MAXN], lst[2 * MAXN], depthOf[2 * MAXN];
void dfs(int u, int d) { if (L[u] < 0) { depthOf[u] = d; return; } dfs(L[u], d + 1); dfs(R[u], d + 1); }
long long C[MAXN][MAXN], pre[MAXN + 1];
int main() {
    int n; long long w[MAXN];
    printf("Enter n: "); scanf("%d", &n);
    printf("Enter %d weights in key order: ", n);
    for (int i = 0; i < n; i++) { scanf("%lld", &w[i]); wt[i] = w[i]; L[i] = R[i] = -1; lst[i] = i; }
    int m = n, cnt = n;
    while (m > 1) {
        int k = 1;                                    /* first k with w[k-1] <= w[k+1] (right sentinel = infinity) */
        while (k < m - 1 && wt[lst[k - 1]] > wt[lst[k + 1]]) k++;
        int a = lst[k - 1], b = lst[k];
        wt[cnt] = wt[a] + wt[b]; L[cnt] = a; R[cnt] = b;
        for (int i = k + 1; i < m; i++) lst[i - 2] = lst[i];       /* delete the pair */
        m -= 2;
        int j = k - 2; while (j >= 0 && wt[lst[j]] < wt[cnt]) j--;  /* nearest item on the left with weight >= sum */
        for (int i = m; i > j + 1; i--) lst[i] = lst[i - 1];
        lst[j + 1] = cnt++; m++;
    }
    dfs(lst[0], 0);
    int d[MAXN]; long long cost = 0;
    for (int i = 0; i < n; i++) { d[i] = depthOf[i]; cost += w[i] * d[i]; }
    if (n == 1) d[0] = 1;
    /* phase 3: leftmost-available codewords for the depths d[] */
    unsigned long long c = 0, code[MAXN]; int valid = 1;
    for (int i = 0; i < n; i++) {
        if (i) { c = c + 1; if (d[i] >= d[i - 1]) c <<= (d[i] - d[i - 1]); else { if (c & ((1ULL << (d[i - 1] - d[i])) - 1)) valid = 0; c >>= (d[i - 1] - d[i]); } }
        code[i] = c;
    }
    printf("\n%-5s %-8s %-6s %s\n", "Key", "Weight", "Depth", "Code");
    for (int i = 0; i < n; i++) {
        char buf[70]; for (int b = 0; b < d[i]; b++) buf[b] = ((code[i] >> (d[i] - 1 - b)) & 1) ? '1' : '0'; buf[d[i]] = 0;
        printf("%-5d %-8lld %-6d %s\n", i + 1, w[i], d[i], buf);
    }
    /* checks: prefix-free + order preserving, and optimality via interval DP */
    for (int i = 0; i < n && valid; i++) for (int j = i + 1; j < n; j++) {
        int s = d[i] < d[j] ? d[i] : d[j];
        unsigned long long x = code[i] >> (d[i] - s), y = code[j] >> (d[j] - s);
        if (x >= y) { valid = 0; break; }               /* equal prefix => not prefix-free, larger => not alphabetic */
    }
    pre[0] = 0; for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + w[i];
    for (int len = 2; len <= n; len++) for (int i = 0; i + len <= n; i++) {
        int j = i + len - 1; long long best = INF;
        for (int k = i; k < j; k++) { long long v = C[i][k] + C[k + 1][j]; if (v < best) best = v; }
        C[i][j] = best + pre[j + 1] - pre[i];
    }
    printf("\nWeighted path length sum(w_i * depth_i) = %lld\n", cost);
    printf("Interval-DP optimum (O(n^3) check)      = %lld -> %s\n", n > 1 ? C[0][n - 1] : 0LL, (n == 1 || C[0][n - 1] == cost) ? "MATCH" : "MISMATCH");
    printf("Prefix-free & alphabetic check          = %s\n", valid ? "PASS" : "FAIL");
    return 0;
}
