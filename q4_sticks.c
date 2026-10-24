/* Q4: Minimum cost to connect sticks (min-heap, Huffman-style merging) */
#include <stdio.h>
#include <stdlib.h>
long long h[100005]; int hs = 0;
void push(long long x) { int i = hs++; h[i] = x; while (i && h[i] < h[(i - 1) / 2]) { int p = (i - 1) / 2; long long t = h[i]; h[i] = h[p]; h[p] = t; i = p; } }
long long pop() { long long r = h[0]; h[0] = h[--hs]; int i = 0;
    for (;;) { int l = 2 * i + 1, rr = l + 1, m = i;
        if (l < hs && h[l] < h[m]) m = l; if (rr < hs && h[rr] < h[m]) m = rr;
        if (m == i) break; long long t = h[i]; h[i] = h[m]; h[m] = t; i = m; }
    return r; }
int main() {
    int n; printf("Enter n: "); scanf("%d", &n);
    printf("Enter %d stick lengths: ", n);
    for (int i = 0; i < n; i++) { long long x; scanf("%lld", &x); push(x); }
    long long cost = 0; int step = 1;
    while (hs > 1) {
        long long x = pop(), y = pop(); cost += x + y; push(x + y);
        printf("Step %d: join %lld + %lld -> %lld (cost %lld)\n", step++, x, y, x + y, x + y);
    }
    printf("Minimum total cost = %lld\n", cost);
    return 0;
}
