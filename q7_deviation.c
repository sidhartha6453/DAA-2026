/* Q7: Minimise deviation in array (normalise to even values, shrink the maximum with a max-heap) */
#include <stdio.h>
#include <stdlib.h>
#define MAXN 100005
long long h[MAXN]; int hs = 0;
void push(long long x) { int i = hs++; h[i] = x; while (i && h[i] > h[(i - 1) / 2]) { int p = (i - 1) / 2; long long t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
long long pop() { long long r = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, rr = l + 1, m = i;
        if (l < hs && h[l] > h[m]) m = l; if (rr < hs && h[rr] > h[m]) m = rr;
        if (m == i) break; long long t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return r; }

long long A[MAXN], best;
/* brute force: every element can only take values o*2^j (o = odd part), enumerate all combinations */
void brute(int i, int n, long long mn, long long mx) {
    if (i == n) { if (mx - mn < best) best = mx - mn; return; }
    long long o = A[i]; int e = 0; while (o % 2 == 0) { o /= 2; e++; }
    if (A[i] % 2) e = 1;                      /* odd: may double once */
    for (int j = 0; j <= e; j++) { long long v = o << j; brute(i + 1, n, v < mn ? v : mn, v > mx ? v : mx); }
}

int main() {
    int n; printf("Enter n: "); scanf("%d", &n);
    printf("Enter %d positive integers: ", n);
    long long mn = (long long)4e18;
    for (int i = 0; i < n; i++) { scanf("%lld", &A[i]); long long x = A[i] % 2 ? A[i] * 2 : A[i]; push(x); if (x < mn) mn = x; }
    long long ans = h[0] - mn;
    while (h[0] % 2 == 0) {                         /* max is even -> can still be reduced */
        long long x = pop() / 2; push(x); if (x < mn) mn = x;
        if (h[0] - mn < ans) ans = h[0] - mn;
    }
    printf("Minimum deviation = %lld\n", ans);
    if (n <= 12) { best = (long long)4e18; brute(0, n, (long long)4e18, 0); printf("Brute force            = %lld -> %s\n", best, best == ans ? "MATCH" : "MISMATCH"); }
    return 0;
}
