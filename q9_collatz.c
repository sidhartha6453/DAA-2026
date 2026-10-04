// Input: "1 n" for one trajectory, or "2 a b" for an interval.  Overflow-safe, memoised.
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef unsigned long long u64;

int next(u64 *n) {                                     // one step; 0 on overflow
    if (*n % 2 == 0) { *n /= 2; return 1; }
    if (*n > (ULLONG_MAX - 1) / 3) return 0;
    *n = 3 * *n + 1; return 1;
}
void single(u64 n) {
    u64 peak = n; int steps = 0;
    printf("%llu", n);
    while (n != 1) {
        if (!next(&n)) return (void)puts("\n[overflow]");
        printf(" -> %llu", n);
        if (n > peak) peak = n;
        steps++;
    }
    printf("\nSteps = %d, peak = %llu\n", steps, peak);
}
void interval(u64 a, u64 b) {
    int *st = calloc(b + 1, sizeof(int));              // cache: steps for values <= b
    u64 best = a; int bs = -1;
    for (u64 s = a; s <= b; s++) {
        u64 n = s; int c = 0;
        while (n != 1 && !(n <= b && st[n])) { if (!next(&n)) { c = -1; break; } c++; }
        if (c < 0) continue;
        st[s] = c + (n == 1 ? 0 : st[n]);
        if (st[s] > bs) bs = st[s], best = s;
    }
    printf("Longest in [%llu,%llu]: start %llu, %d steps\n", a, b, best, bs);
    free(st);
}
int main() {
    int mode; u64 a, b;
    scanf("%d %llu", &mode, &a);
    if (mode == 1) single(a);
    else scanf("%llu", &b), interval(a, b);
}
